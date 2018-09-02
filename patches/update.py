import os, sys
from common import *

def update_repos(projects, remotes, dry_run = False, repos = []):
	ANDROID_BUILD_TOP, MANIFEST, ROOT = setup_env()
	if not repos:
		repos = PROJECTS

	for p in projects:
		path = p.get("path")
		#if (not path in repos) or path.startswith("#"):
		#	continue
		remote = p.get("remote") or remotes["default"].name
		if p.get("revision"):
			revision = "%s/%s" % (remote, p.get("revision"))
		else:
			revision = remotes["default"].revision
		repo_realpath = os.path.join(ANDROID_BUILD_TOP, path)
		out_path = os.path.join(ROOT, path)
		p = Popen("git -C %s rev-parse %s" % (repo_realpath, revision), shell=True, stdin=PIPE,
						stderr = PIPE, stdout = PIPE, close_fds=True)
		rev_parse = p.stdout.read().split("\n")[0]
		if not os.path.exists(path):
			os.makedirs(path)

		if rev_parse:
			p = Popen("git -C %s merge-base HEAD %s" % (repo_realpath, rev_parse), shell=True, stdin=PIPE,
                               	                stderr = PIPE, stdout = PIPE, close_fds=True)
			if not (p.stdout.read()):
				print("error: %s: no common merge base found for HEAD and manifest revision (%s), skipping this repo" % (path, revision))
				continue

			if not dry_run:
				for i in os.listdir(path):
					subfile = os.path.join(path, i)
					if os.path.isfile(subfile):
						os.remove(os.path.join(path, i))
				p = Popen("git -C %s format-patch %s -o %s" % (repo_realpath, rev_parse, out_path), shell=True, stdin=PIPE,
                                	                stderr = PIPE, stdout = PIPE, close_fds=True)
			print("git -C %s format-patch %s -o %s" % (repo_realpath, rev_parse, out_path))
			if not dry_run:
				print(p.stdout.read())
		else:
			err_msg = "error: %s is not a git tree or broken\n" % path
			sys.stderr.write(err_msg)
			print("# %s" % err_msg)


def main(dry_run = False):
	projects, remotes, default = parse_manifests()
	argv = sys.argv
	if "--dry-run" in argv:
		dry_run = True
		del argv[argv.index("--dry-run")]

	if len(argv) > 1:
		update_repos(projects, remotes, dry_run, repos = argv[1:])
		return

	update_repos(projects, remotes, dry_run)


if __name__ == "__main__":
	main()
