int parse_server_match_testspec(struct connection_info *ci, char *spec)
{
	char *p;

	while ((p = strsep(&spec, ",")) && *p != '\0') {
		if (strncmp(p, "addr=", 5) == 0) {
			ci->address = xstrdup(p + 5);
		} else if (strncmp(p, "host=", 5) == 0) {
			ci->host = xstrdup(p + 5);
		} else if (strncmp(p, "user=", 5) == 0) {
			ci->user = xstrdup(p + 5);
		} else if (strncmp(p, "laddr=", 6) == 0) {
			ci->laddress = xstrdup(p + 6);
		} else if (strncmp(p, "lport=", 6) == 0) {
			ci->lport = a2port(p + 6);
			if (ci->lport == -1) {
				fprintf(stderr, "Invalid port '%s' in test mode"
				   " specification %s\n", p+6, p);
				return -1;
			}
		} else {
			fprintf(stderr, "Invalid test mode specification %s\n",
			   p);
			return -1;
		}
	}
	return 0;
}
