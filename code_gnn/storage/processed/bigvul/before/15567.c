resolve_canonicalize(char **hostp, int port)
{
	int i, ndots;
	char *cp, *fullhost, newname[NI_MAXHOST];
	struct addrinfo *addrs;

	if (options.canonicalize_hostname == SSH_CANONICALISE_NO)
		return NULL;

	 
	if (!option_clear_or_none(options.proxy_command) &&
	    options.canonicalize_hostname != SSH_CANONICALISE_ALWAYS)
		return NULL;

	 
	if ((addrs = resolve_addr(*hostp, port,
	    newname, sizeof(newname))) != NULL) {
		debug2("%s: hostname %.100s is address", __func__, *hostp);
		if (strcasecmp(*hostp, newname) != 0) {
			debug2("%s: canonicalised address \"%s\" => \"%s\"",
			    __func__, *hostp, newname);
			free(*hostp);
			*hostp = xstrdup(newname);
		}
		return addrs;
	}

	 
	if ((*hostp)[strlen(*hostp) - 1] == '.') {
		debug3("%s: name is fully qualified", __func__);
		fullhost = xstrdup(*hostp);
		if ((addrs = resolve_host(fullhost, port, 0,
		    newname, sizeof(newname))) != NULL)
			goto found;
		free(fullhost);
		goto notfound;
	}

	 
	ndots = 0;
	for (cp = *hostp; *cp != '\0'; cp++) {
		if (*cp == '.')
			ndots++;
	}
	if (ndots > options.canonicalize_max_dots) {
		debug3("%s: not canonicalizing hostname \"%s\" (max dots %d)",
		    __func__, *hostp, options.canonicalize_max_dots);
		return NULL;
	}
	 
	for (i = 0; i < options.num_canonical_domains; i++) {
		*newname = '\0';
		xasprintf(&fullhost, "%s.%s.", *hostp,
		    options.canonical_domains[i]);
		debug3("%s: attempting \"%s\" => \"%s\"", __func__,
		    *hostp, fullhost);
		if ((addrs = resolve_host(fullhost, port, 0,
		    newname, sizeof(newname))) == NULL) {
			free(fullhost);
			continue;
		}
 found:
		 
		fullhost[strlen(fullhost) - 1] = '\0';
		 
		if (!check_follow_cname(&fullhost, newname)) {
			debug("Canonicalized hostname \"%s\" => \"%s\"",
			    *hostp, fullhost);
		}
		free(*hostp);
		*hostp = fullhost;
		return addrs;
	}
 notfound:
	if (!options.canonicalize_fallback_local)
		fatal("%s: Could not resolve host \"%s\"", __progname, *hostp);
	debug2("%s: host %s not found in any suffix", __func__, *hostp);
	return NULL;
}
