static int good_hostname(const char *name)
{

	for (;;) {
		name = valid_domain_label(name);
		if (!name)
			return 0;
		if (!name[0])
			return 1;
		name++;
		if (*name == '\0')
			return 1;  
	}
}
