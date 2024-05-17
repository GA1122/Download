int chown_mapped_root(char *path, struct lxc_conf *conf)
{
	uid_t rootuid;
	gid_t rootgid;
	pid_t pid;
	unsigned long val;
	char *chownpath = path;

	if (!get_mapped_rootid(conf, ID_TYPE_UID, &val)) {
		ERROR("No mapping for container root");
		return -1;
	}
	rootuid = (uid_t) val;
	if (!get_mapped_rootid(conf, ID_TYPE_GID, &val)) {
		ERROR("No mapping for container root");
		return -1;
	}
	rootgid = (gid_t) val;

	 
	if (strncmp(path, "overlayfs:", 10) == 0 || strncmp(path, "aufs:", 5) == 0) {
		chownpath = strchr(path, ':');
		if (!chownpath) {
			ERROR("Bad overlay path: %s", path);
			return -1;
		}
		chownpath = strchr(chownpath+1, ':');
		if (!chownpath) {
			ERROR("Bad overlay path: %s", path);
			return -1;
		}
		chownpath++;
	}
	path = chownpath;
	if (geteuid() == 0) {
		if (chown(path, rootuid, rootgid) < 0) {
			ERROR("Error chowning %s", path);
			return -1;
		}
		return 0;
	}

	if (rootuid == geteuid()) {
		INFO("%s: container root is our uid;  no need to chown" ,__func__);
		return 0;
	}

	pid = fork();
	if (pid < 0) {
		SYSERROR("Failed forking");
		return -1;
	}
	if (!pid) {
		int hostuid = geteuid(), hostgid = getegid(), ret;
		struct stat sb;
		char map1[100], map2[100], map3[100], map4[100], map5[100];
		char ugid[100];
		char *args1[] = { "lxc-usernsexec", "-m", map1, "-m", map2,
				"-m", map3, "-m", map5,
				"--", "chown", ugid, path, NULL };
		char *args2[] = { "lxc-usernsexec", "-m", map1, "-m", map2,
				"-m", map3, "-m", map4, "-m", map5,
				"--", "chown", ugid, path, NULL };

		if (stat(path, &sb) < 0) {
			ERROR("Error stat %s", path);
			return -1;
		}

		 
		if (sb.st_uid == geteuid() &&
				mapped_hostid(sb.st_gid, conf, ID_TYPE_GID) < 0 &&
				chown(path, -1, hostgid) < 0) {
			ERROR("Failed chgrping %s", path);
			return -1;
		}

		ret = snprintf(map1, 100, "u:0:%d:1", rootuid);
		if (ret < 0 || ret >= 100) {
			ERROR("Error uid printing map string");
			return -1;
		}

		ret = snprintf(map2, 100, "u:%d:%d:1", hostuid, hostuid);
		if (ret < 0 || ret >= 100) {
			ERROR("Error uid printing map string");
			return -1;
		}

		ret = snprintf(map3, 100, "g:0:%d:1", rootgid);
		if (ret < 0 || ret >= 100) {
			ERROR("Error gid printing map string");
			return -1;
		}

		ret = snprintf(map4, 100, "g:%d:%d:1", (gid_t)sb.st_gid,
				rootgid + (gid_t)sb.st_gid);
		if (ret < 0 || ret >= 100) {
			ERROR("Error gid printing map string");
			return -1;
		}

		ret = snprintf(map5, 100, "g:%d:%d:1", hostgid, hostgid);
		if (ret < 0 || ret >= 100) {
			ERROR("Error gid printing map string");
			return -1;
		}

		ret = snprintf(ugid, 100, "0:%d", (gid_t)sb.st_gid);
		if (ret < 0 || ret >= 100) {
			ERROR("Error owner printing format string for chown");
			return -1;
		}

		if (hostgid == sb.st_gid)
			ret = execvp("lxc-usernsexec", args1);
		else
			ret = execvp("lxc-usernsexec", args2);
		SYSERROR("Failed executing usernsexec");
		exit(1);
	}
	return wait_for_pid(pid);
}
