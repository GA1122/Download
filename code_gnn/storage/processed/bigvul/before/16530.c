set_runtime_config(char *admin, char *config)
{
	int i;

	if (!admin || !admin[0] || !enable_runtime) {
		if (admin)  { free(admin);  }
		if (config) { free(config); }
		return -1;
	}

	if (config && config[0]) {
		for (i=0; i <= rArray.getlast(); i++) {
			if (strcmp(rArray[i].admin, admin) == MATCH) {
				free(admin);
				free(rArray[i].config);
				rArray[i].config = config;
				return 0;
			}
		}
		rArray[i].admin = admin;
		rArray[i].config = config;
	} else {
		for (i=0; i <= rArray.getlast(); i++) {
			if (strcmp(rArray[i].admin, admin) == MATCH) {
				free(admin);
				if (config) free(config);
				free(rArray[i].admin);
				free(rArray[i].config);
				rArray[i] = rArray[rArray.getlast()];
				rArray[rArray.getlast()].initialize();
				rArray.truncate(rArray.getlast()-1);
				return 0;
			}
		}
	}

	return 0;
}
