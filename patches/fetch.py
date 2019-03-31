from __future__ import print_function
import os, sys, traceback
from common import *

def execute(cmd):
	p = Popen(cmd, shell=True, stdin=PIPE,
		stderr = PIPE, stdout = PIPE, close_fds=True, universal_newlines=True)
	for stdout_line in iter(p.stdout.readline, ""):
		yield stdout_line
	p.stdout.close()
	return_code = p.wait()
	#if return_code:
	#	raise CalledProcessError(return_code, cmd)

def fetch_unshallow(projects, remotes, dry_run = False):
	ANDROID_BUILD_TOP, MANIFEST, ROOT = setup_env()
	for p in projects:
		path = p.get("path")
		if (not path in PROJECTS) or path.startswith("#"):
			continue

		remote = p.get("remote") or remotes["default"].name
		#print(p.get("revision"), remotes["default"].revision)
		if p.get("revision"):
			revision = p.get("revision")
		else:
			revision = remotes["default"].revision
			if "/" in revision:
				remote, revision = revision.split("/")

		repo_realpath = os.path.join(ANDROID_BUILD_TOP, path)
		out_path = os.path.join(ROOT, path)
		cmd = "git -C %s fetch %s %s --unshallow" % (repo_realpath, remote, revision)
		print(cmd)
		if not dry_run:
			try:
				exec_iter = execute(cmd)
			except CalledProcessError:
				print(traceback.format_exc())
				continue
			for line in execute(cmd):
				print(line, end="")

def main(dry_run = False):
	projects, remotes, default = parse_manifests()
	if "--dry-run" in sys.argv:
		dry_run = True
	fetch_unshallow(projects, remotes, dry_run)


if __name__ == "__main__":
	main()
