order_hostkeyalgs(char *host, struct sockaddr *hostaddr, u_short port)
{
	char *oavail, *avail, *first, *last, *alg, *hostname, *ret;
	size_t maxlen;
	struct hostkeys *hostkeys;
	int ktype;
	u_int i;

	 
	get_hostfile_hostname_ipaddr(host, hostaddr, port, &hostname, NULL);
	hostkeys = init_hostkeys();
	for (i = 0; i < options.num_user_hostfiles; i++)
		load_hostkeys(hostkeys, hostname, options.user_hostfiles[i]);
	for (i = 0; i < options.num_system_hostfiles; i++)
		load_hostkeys(hostkeys, hostname, options.system_hostfiles[i]);

	oavail = avail = xstrdup(KEX_DEFAULT_PK_ALG);
	maxlen = strlen(avail) + 1;
	first = xmalloc(maxlen);
	last = xmalloc(maxlen);
	*first = *last = '\0';

#define ALG_APPEND(to, from) \
	do { \
		if (*to != '\0') \
			strlcat(to, ",", maxlen); \
		strlcat(to, from, maxlen); \
	} while (0)

	while ((alg = strsep(&avail, ",")) && *alg != '\0') {
		if ((ktype = sshkey_type_from_name(alg)) == KEY_UNSPEC)
			fatal("%s: unknown alg %s", __func__, alg);
		if (lookup_key_in_hostkeys_by_type(hostkeys,
		    sshkey_type_plain(ktype), NULL))
			ALG_APPEND(first, alg);
		else
			ALG_APPEND(last, alg);
	}
#undef ALG_APPEND
	xasprintf(&ret, "%s%s%s", first,
	    (*first == '\0' || *last == '\0') ? "" : ",", last);
	if (*first != '\0')
		debug3("%s: prefer hostkeyalgs: %s", __func__, first);

	free(first);
	free(last);
	free(hostname);
	free(oavail);
	free_hostkeys(hostkeys);

	return ret;
}
