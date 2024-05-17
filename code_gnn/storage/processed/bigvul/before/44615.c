static int parse_cap(const char *cap)
{
	char *ptr = NULL;
	int i, capid = -1;

	if (!strcmp(cap, "none"))
		return -2;

	for (i = 0; i < sizeof(caps_opt)/sizeof(caps_opt[0]); i++) {

		if (strcmp(cap, caps_opt[i].name))
			continue;

		capid = caps_opt[i].value;
		break;
	}

	if (capid < 0) {
		 
		errno = 0;
		capid = strtol(cap, &ptr, 10);
		if (!ptr || *ptr != '\0' || errno != 0)
			 
			capid = -1;
		else if (capid > lxc_caps_last_cap())
			 
			capid = -1;
	}

	return capid;
}
