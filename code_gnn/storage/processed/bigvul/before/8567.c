static void ssh_hostport_setup(const char *host, int port, Conf *conf,
                               char **savedhost, int *savedport,
                               char **loghost_ret)
{
    char *loghost = conf_get_str(conf, CONF_loghost);
    if (loghost_ret)
        *loghost_ret = loghost;

    if (*loghost) {
	char *tmphost;
        char *colon;

        tmphost = dupstr(loghost);
	*savedport = 22;	        

	 
	colon = host_strrchr(tmphost, ':');
	if (colon && colon == host_strchr(tmphost, ':')) {
	    *colon++ = '\0';
	    if (*colon)
		*savedport = atoi(colon);
	}

        *savedhost = host_strduptrim(tmphost);
        sfree(tmphost);
    } else {
	*savedhost = host_strduptrim(host);
	if (port < 0)
	    port = 22;		        
	*savedport = port;
    }
}