static size_t resp_hdr_cb(void *ptr, size_t size, size_t nmemb, void *user_data)
{
	struct header_info *hi = (struct header_info *)user_data;
	size_t remlen, slen, ptrlen = size * nmemb;
	char *rem, *val = NULL, *key = NULL;
	void *tmp;

	val = (char *)calloc(1, ptrlen);
	key = (char *)calloc(1, ptrlen);
	if (!key || !val)
		goto out;

	tmp = memchr(ptr, ':', ptrlen);
	if (!tmp || (tmp == ptr))	 
		goto out;
	slen = (uint8_t*)tmp - (uint8_t*)ptr;
	if ((slen + 1) == ptrlen)	 
		goto out;
	memcpy(key, ptr, slen);		 
	key[slen] = 0;

	rem = (char*)ptr + slen + 1;	 
	remlen = ptrlen - slen - 1;
	while ((remlen > 0) && (isspace(*rem))) {
		remlen--;
		rem++;
	}

	memcpy(val, rem, remlen);	 
	val[remlen] = 0;
	while ((*val) && (isspace(val[strlen(val) - 1])))
		val[strlen(val) - 1] = 0;

	if (!*val)			 
		goto out;

	if (opt_protocol)
		applog(LOG_DEBUG, "HTTP hdr(%s): %s", key, val);

	if (!strcasecmp("X-Roll-Ntime", key)) {
		hi->hadrolltime = true;
		if (!strncasecmp("N", val, 1))
			applog(LOG_DEBUG, "X-Roll-Ntime: N found");
		else {
			hi->canroll = true;

			 
			if (strlen(val) > 7 && !strncasecmp("expire=", val, 7)) {
				sscanf(val + 7, "%d", &hi->rolltime);
				hi->hadexpire = true;
			} else
				hi->rolltime = opt_scantime;
			applog(LOG_DEBUG, "X-Roll-Ntime expiry set to %d", hi->rolltime);
		}
	}

	if (!strcasecmp("X-Long-Polling", key)) {
		hi->lp_path = val;	 
		val = NULL;
	}

	if (!strcasecmp("X-Reject-Reason", key)) {
		hi->reason = val;	 
		val = NULL;
	}

	if (!strcasecmp("X-Stratum", key)) {
		hi->stratum_url = val;
		val = NULL;
	}

out:
	free(key);
	free(val);
	return ptrlen;
}