static void save_nogroups(void) {
	if (arg_nogroups == 0)
		return;

	FILE *fp = fopen(RUN_GROUPS_CFG, "w");
	if (fp) {
		fprintf(fp, "\n");
		SET_PERMS_STREAM(fp, 0, 0, 0644);  
		fclose(fp);
	}
	else {
		fprintf(stderr, "Error: cannot save nogroups state\n");
		exit(1);
	}
}