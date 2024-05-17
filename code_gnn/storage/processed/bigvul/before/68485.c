 server_file_access(struct httpd *env, struct client *clt,
    char *path, size_t len)
{
	struct http_descriptor	*desc = clt->clt_descreq;
	struct server_config	*srv_conf = clt->clt_srv_conf;
	struct stat		 st;
	struct kv		*r, key;
	char			*newpath, *encodedpath;
	int			 ret;

	errno = 0;

	if (access(path, R_OK) == -1) {
		goto fail;
	} else if (stat(path, &st) == -1) {
		goto fail;
	} else if (S_ISDIR(st.st_mode)) {
		 
		if (srv_conf->flags & SRVFLAG_NO_INDEX) {
			errno = EACCES;
			goto fail;
		}

		if (desc->http_path_alias != NULL) {
			 
			errno = EINVAL;
			goto fail;
		}

		 
		if (path[strlen(path) - 1] != '/') {
			if ((encodedpath = url_encode(desc->http_path)) == NULL)
				return (500);
			if (asprintf(&newpath, "http%s://%s%s/",
			    srv_conf->flags & SRVFLAG_TLS ? "s" : "",
			    desc->http_host, encodedpath) == -1) {
				free(encodedpath);
				return (500);
			}
			free(encodedpath);

			 
			desc->http_path_alias = newpath;

			 
			return (301);
		}

		 
		if (asprintf(&newpath, "%s%s", desc->http_path,
		    srv_conf->index) == -1)
			return (500);
		desc->http_path_alias = newpath;
		if (server_getlocation(clt, newpath) != srv_conf) {
			 
			return (server_file(env, clt));
		}

		 
		if (strlcat(path, srv_conf->index, len) >= len) {
			errno = EACCES;
			goto fail;
		}

		ret = server_file_access(env, clt, path, len);
		if (ret == 404) {
			 
			if ((srv_conf->flags & SRVFLAG_AUTO_INDEX) == 0) {
				errno = EACCES;
				goto fail;
			}

			return (server_file_index(env, clt, &st));
		}
		return (ret);
	} else if (!S_ISREG(st.st_mode)) {
		 
		errno = EACCES;
		goto fail;
	}

	key.kv_key = "Range";
	r = kv_find(&desc->http_headers, &key);
	if (r != NULL)
		return (server_partial_file_request(env, clt, path, &st,
		    r->kv_value));
	else
		return (server_file_request(env, clt, path, &st));

 fail:
	switch (errno) {
	case ENOENT:
	case ENOTDIR:
		return (404);
	case EACCES:
		return (403);
	default:
		return (500);
	}

	 
}
