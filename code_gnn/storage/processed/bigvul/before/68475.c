int fs_copydir(const char *path, const struct stat *st, int ftype, struct FTW *sftw) {
	(void) st;
	(void) sftw;
	if (size_limit_reached)
		return 0;

	struct stat s;
	char *dest;
	if (asprintf(&dest, "%s%s", RUN_HOME_DIR, path + strlen(cfg.homedir)) == -1)
		errExit("asprintf");

	if (stat(dest, &s) == 0) {
		free(dest);
		return 0;
	}
	
	if (stat(path, &s) != 0) {
		free(dest);
		return 0;
	}

	if (s.st_uid != firejail_uid || s.st_gid != firejail_gid) {
		free(dest);
		return 0;
	}

	if ((s.st_size + size_cnt) > PRIVATE_COPY_LIMIT) {
		size_limit_reached = 1;
		free(dest);
		return 0;
	}

	file_cnt++;
	size_cnt += s.st_size;

	if(ftype == FTW_F)
		copy_file(path, dest, firejail_uid, firejail_gid, s.st_mode);
	else if (ftype == FTW_D) {
		if (mkdir(dest, s.st_mode) == -1)
			errExit("mkdir");
		if (chmod(dest, s.st_mode) < 0) {
			fprintf(stderr, "Error: cannot change mode for %s\n", path);
			exit(1);
		}
		if (chown(dest, firejail_uid, firejail_gid) < 0) {
			fprintf(stderr, "Error: cannot change ownership for %s\n", path);
			exit(1);
		}

#if 0
struct stat s2;		
if (stat(dest, &s2) == 0) {
    printf("%s\t", dest);
    printf((S_ISDIR(s.st_mode))  ? "d" : "-");
    printf((s.st_mode & S_IRUSR) ? "r" : "-");
    printf((s.st_mode & S_IWUSR) ? "w" : "-");
    printf((s.st_mode & S_IXUSR) ? "x" : "-");
    printf((s.st_mode & S_IRGRP) ? "r" : "-");
    printf((s.st_mode & S_IWGRP) ? "w" : "-");
    printf((s.st_mode & S_IXGRP) ? "x" : "-");
    printf((s.st_mode & S_IROTH) ? "r" : "-");
    printf((s.st_mode & S_IWOTH) ? "w" : "-");
    printf((s.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}
#endif		
		
		fs_logger2("clone", path);
	}		
		
	free(dest);
	return(0);
}