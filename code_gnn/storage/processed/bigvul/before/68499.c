server_http_parsehost(char *host, char *buf, size_t len, int *portval)
{
	char		*start, *end, *port;
	const char	*errstr = NULL;

	if (strlcpy(buf, host, len) >= len) {
		log_debug("%s: host name too long", __func__);
		return (NULL);
	}

	start = buf;
	end = port = NULL;

	if (*start == '[' && (end = strchr(start, ']')) != NULL) {
		 
		start++;
		*end++ = '\0';
		if ((port = strchr(end, ':')) == NULL || *port == '\0')
			port = NULL;
		else
			port++;
		memmove(buf, start, strlen(start) + 1);
	} else if ((end = strchr(start, ':')) != NULL) {
		 
		*end++ = '\0';
		port = end;
	} else {
		 
		port = NULL;
	}

	if (port != NULL) {
		 
		*portval = strtonum(port, 0, 0xffff, &errstr);
		if (errstr != NULL) {
			log_debug("%s: invalid port: %s", __func__,
			    strerror(errno));
			return (NULL);
		}
		*portval = htons(*portval);
	} else {
		 
		*portval = -1;
	}

	return (start);
}
