server_log_http(struct client *clt, unsigned int code, size_t len)
{
	static char		 tstamp[64];
	static char		 ip[INET6_ADDRSTRLEN];
	time_t			 t;
	struct kv		 key, *agent, *referrer;
	struct tm		*tm;
	struct server_config	*srv_conf;
	struct http_descriptor	*desc;
	int			 ret = -1;
	char			*user = NULL;
	char			*path = NULL;
	char			*query = NULL;
	char			*version = NULL;
	char			*referrer_v = NULL;
	char			*agent_v = NULL;

	if ((srv_conf = clt->clt_srv_conf) == NULL)
		return (-1);
	if ((srv_conf->flags & SRVFLAG_LOG) == 0)
		return (0);
	if ((desc = clt->clt_descreq) == NULL)
		return (-1);

	if ((t = time(NULL)) == -1)
		return (-1);
	if ((tm = localtime(&t)) == NULL)
		return (-1);
	if (strftime(tstamp, sizeof(tstamp), "%d/%b/%Y:%H:%M:%S %z", tm) == 0)
		return (-1);

	if (print_host(&clt->clt_ss, ip, sizeof(ip)) == NULL)
		return (-1);

	 
	switch (srv_conf->logformat) {
	case LOG_FORMAT_COMMON:
		 
		if (clt->clt_remote_user &&
		    stravis(&user, clt->clt_remote_user, HTTPD_LOGVIS) == -1)
			goto done;
		if (desc->http_version &&
		    stravis(&version, desc->http_version, HTTPD_LOGVIS) == -1)
			goto done;

		 
		if (desc->http_path &&
		    (path = url_encode(desc->http_path)) == NULL)
			goto done;
		if (desc->http_query &&
		    (query = url_encode(desc->http_query)) == NULL)
			goto done;

		ret = evbuffer_add_printf(clt->clt_log,
		    "%s %s - %s [%s] \"%s %s%s%s%s%s\" %03d %zu\n",
		    srv_conf->name, ip, clt->clt_remote_user == NULL ? "-" :
		    user, tstamp,
		    server_httpmethod_byid(desc->http_method),
		    desc->http_path == NULL ? "" : path,
		    desc->http_query == NULL ? "" : "?",
		    desc->http_query == NULL ? "" : query,
		    desc->http_version == NULL ? "" : " ",
		    desc->http_version == NULL ? "" : version,
		    code, len);

		break;

	case LOG_FORMAT_COMBINED:
		key.kv_key = "Referer";  
		if ((referrer = kv_find(&desc->http_headers, &key)) != NULL &&
		    referrer->kv_value == NULL)
			referrer = NULL;

		key.kv_key = "User-Agent";
		if ((agent = kv_find(&desc->http_headers, &key)) != NULL &&
		    agent->kv_value == NULL)
			agent = NULL;

		 
		if (clt->clt_remote_user &&
		    stravis(&user, clt->clt_remote_user, HTTPD_LOGVIS) == -1)
			goto done;
		if (desc->http_version &&
		    stravis(&version, desc->http_version, HTTPD_LOGVIS) == -1)
			goto done;
		if (agent &&
		    stravis(&agent_v, agent->kv_value, HTTPD_LOGVIS) == -1)
			goto done;

		 
		if (desc->http_path &&
		    (path = url_encode(desc->http_path)) == NULL)
			goto done;
		if (desc->http_query &&
		    (query = url_encode(desc->http_query)) == NULL)
			goto done;
		if (referrer &&
		    (referrer_v = url_encode(referrer->kv_value)) == NULL)
			goto done;

		ret = evbuffer_add_printf(clt->clt_log,
		    "%s %s - %s [%s] \"%s %s%s%s%s%s\""
		    " %03d %zu \"%s\" \"%s\"\n",
		    srv_conf->name, ip, clt->clt_remote_user == NULL ? "-" :
		    user, tstamp,
		    server_httpmethod_byid(desc->http_method),
		    desc->http_path == NULL ? "" : path,
		    desc->http_query == NULL ? "" : "?",
		    desc->http_query == NULL ? "" : query,
		    desc->http_version == NULL ? "" : " ",
		    desc->http_version == NULL ? "" : version,
		    code, len,
		    referrer == NULL ? "" : referrer_v,
		    agent == NULL ? "" : agent_v);

		break;

	case LOG_FORMAT_CONNECTION:
		 
		if (desc->http_path &&
		    (path = url_encode(desc->http_path)) == NULL)
			goto done;

		ret = evbuffer_add_printf(clt->clt_log, " [%s]",
		    desc->http_path == NULL ? "" : path);

		break;
	}

done:
	free(user);
	free(path);
	free(query);
	free(version);
	free(referrer_v);
	free(agent_v);

	return (ret);
}
