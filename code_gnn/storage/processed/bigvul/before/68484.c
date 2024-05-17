server_file(struct httpd *env, struct client *clt)
{
	struct http_descriptor	*desc = clt->clt_descreq;
	struct server_config	*srv_conf = clt->clt_srv_conf;
	char			 path[PATH_MAX];
	const char		*stripped, *errstr = NULL;
	int			 ret = 500;

	if (srv_conf->flags & SRVFLAG_FCGI)
		return (server_fcgi(env, clt));

	 
	stripped = server_root_strip(
	    desc->http_path_alias != NULL ?
	    desc->http_path_alias : desc->http_path,
	    srv_conf->strip);
	if ((size_t)snprintf(path, sizeof(path), "%s%s",
	    srv_conf->root, stripped) >= sizeof(path)) {
		errstr = desc->http_path;
		goto abort;
	}

	 
	if ((ret = server_file_access(env, clt, path, sizeof(path))) > 0) {
		errstr = desc->http_path_alias != NULL ?
		    desc->http_path_alias : desc->http_path;
		goto abort;
	}

	return (ret);

 abort:
	if (errstr == NULL)
		errstr = strerror(errno);
	server_abort_http(clt, ret, errstr);
	return (-1);
}