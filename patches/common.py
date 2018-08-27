#!/usr/bin/env python2

import sys, os
import glob
import xml.etree.ElementTree as ET
from subprocess import Popen, PIPE, CalledProcessError

PROJECTS="""art
bionic
#bootable/recovery
build/make
build/soong
#external/boringssl
#external/busybox
#external/libpng
#external/minijail
external/selinux
#external/zlib
frameworks/av
frameworks/base
frameworks/native
#frameworks/opt/net/wifi
#frameworks/opt/telephony
#frameworks/rs
#frameworks/support
#hardware/interfaces
hardware/libhardware
#hardware/ril
#libcore
system/core
system/media
system/sepolicy
vendor/lineage""".split("\n")

class Remote:
	def __init__(self, tag_remote, tag_default):
		self.name = tag_remote.get("name")
		self.revision = tag_remote.get("revision")
		if tag_default.get("remote") == self.name:
			default_revision = tag_default.get("revision")
			if default_revision:
				self.revision = default_revision
		if self.revision and self.revision.startswith("refs/heads/"):
			self.revision = self.revision.replace("refs/heads", self.name)

	def get(self, what):
		if what == "name":
			return self.name
		if what == "revision":
			return self.revision
		return None


class Project:
	def __init__(self, p):
		if not p.get("name") or not p.get("path"):
			raise RuntimeError("please specify project name and path")
		self.name = p.get("name")
		self.path = p.get("path")
		self.remote = p.get("remote")
		self.revision = p.get("revision")

	def get(self, what):
		if what == "name":
			return self.name

		if what == "path":
			return self.path

		if what == "remote":
			return self.remote

		if what == "revision":
			return self.revision

		return None

class Projects:
	def __init__(self, projects):
		self._projects = projects
		self._current = 0

	def __contains__(self, name):
		return (not get_by_name(self, name))

	def __iter__(self):
		return self

	def next(self):
		if self._current >= len(self._projects):
			raise StopIteration
		self._current += 1
		return self._projects[self._current - 1]

	def get_by_name(self, name):
		for p in self._projects:
			if p.name == name:
				return p
		return None

	def get_all_by_name(self, name):
		ret = []
		for p in self._projects:
			if p.name == name:
				ret.append(p)
		return ret

	def get_all_by_path(self, path):
		ret = []
		for p in self._projects:
			if p.path == path:
				ret.append(p)
		return ret

	def set_by_name(self, name, project):
		if self.get_by_name(project.name):
			self.delete(project.name)

		self._projects.append(project)

	def set_by_path(self, path, project):
		if self.get_by_path(project.path):
			self.delete_by_path(project.path)

		self._projects.append(project)

	def get_by_path(self, path):
		for p in self._projects:
			if p.path == path:
				return p
		return None

	def add_projects(self, projects):
		self._projects += projects

	def delete(self, name):
		idx = 0
		for p in self._projects:
			if p.name == name:
				break
			idx += 1
		assert(self._projects[idx].name == name)
		del self._projects[idx]
		#raise KeyError("key %s not found" % name)

	def delete_by_path(self, path):
		idx = 0
		for p in self._projects:
			if p.path == path:
				break
			idx += 1
		assert(self._projects[idx].path == path)
		del self._projects[idx]
		#raise KeyError("key %s not found" % path)

def setup_env():
	ANDROID_BUILD_TOP = os.getenv("ANDROID_BUILD_TOP")
	if not ANDROID_BUILD_TOP:
		print("please set ANDROID_BUILD_TOP environment variable and retry")
		sys.exit(1)

	MANIFEST = os.path.join(ANDROID_BUILD_TOP, ".repo/manifests/default.xml")
	if not os.path.exists(MANIFEST):
		print("fatal error: manifest file %s not found" % MANIFEST)
		sys.exit(1)

	ROOT = os.path.realpath("")
	return ANDROID_BUILD_TOP, MANIFEST, ROOT

def parse_manifests():
	ANDROID_BUILD_TOP, MANIFEST, ROOT = setup_env()

	et_parser = ET.XMLParser(encoding = "utf-8")
	tree = ET.parse(MANIFEST, parser = et_parser)
	default = tree.find("default")
	remotes = [Remote(tag, default) for tag in tree.findall("remote")]
	remotes = dict([(r.name, r) for r in remotes])
	remotes["default"] = remotes[default.get("remote")]
	projects = [Project(p) for p in tree.findall("project")]

	custom_manifests = [os.path.join(ANDROID_BUILD_TOP, ".repo/manifests", m.get("name")) for m in tree.findall("include")]

	local_path = os.path.join(ANDROID_BUILD_TOP, ".repo/local_manifests")
	local_manifests = glob.glob(os.path.join(local_path, "*.xml"))

	removes = []
	for m in local_manifests:
		et_parser = ET.XMLParser(encoding = "utf-8")
		tree = ET.parse(m, parser = et_parser)
		removes += [r.get("name") for r in tree.findall("remove-project")]

	for m in custom_manifests:
		et_parser = ET.XMLParser(encoding = "utf-8")
		tree = ET.parse(m, parser = et_parser)
		tmp_remotes = [Remote(tag, default) for tag in tree.findall("remote")]
		for r in tmp_remotes:
			remotes[r.name] = r
		projects += [Project(p) for p in tree.findall("project")]

	projects = Projects(projects)

	for r_name in removes:
		projects.delete(r_name)

	for m in local_manifests:
		et_parser = ET.XMLParser(encoding = "utf-8")
		tree = ET.parse(m, parser = et_parser)
		tmp_remotes = [Remote(tag, default) for tag in tree.findall("remote")]
		for r in tmp_remotes:
			remotes[r.name] = r
		for p in tree.findall("project"):
			p = Project(p)
			projects.set_by_name(p.name, p)

	return projects, remotes, default

def main(dry_run = False):
	projects, remotes, default = parse_manifests()
	if "--dry-run" in sys.argv:
		dry_run = True
	update_repos(projects, remotes, dry_run)


if __name__ == "__main__":
	main()
