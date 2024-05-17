static void save_umask(void) {
	FILE *fp = fopen(RUN_UMASK_FILE, "wxe");
	if (fp) {
		fprintf(fp, "%o\n", orig_umask);
		SET_PERMS_STREAM(fp, 0, 0, 0644);  
		fclose(fp);
	}
	else {
		fprintf(stderr, "Error: cannot save umask\n");
		exit(1);
	}
}