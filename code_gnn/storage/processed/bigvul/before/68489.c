server_file_request(struct httpd *env, struct client *clt, char *path,
    struct stat *st)
{
	struct server_config	*srv_conf = clt->clt_srv_conf;
	struct media_type	*media;
	const char		*errstr = NULL;
	int			 fd = -1, ret, code = 500;

	if ((ret = server_file_method(clt)) != 0) {
		code = ret;
		goto abort;
	}

	if ((ret = server_file_modified_since(clt->clt_descreq, st)) != -1)
		return (ret);

	 
	if ((fd = open(path, O_RDONLY)) == -1)
		goto abort;

	media = media_find_config(env, srv_conf, path);
	ret = server_response_http(clt, 200, media, st->st_size,
	    MINIMUM(time(NULL), st->st_mtim.tv_sec));
	switch (ret) {
	case -1:
		goto fail;
	case 0:
		 
		close(fd);
		goto done;
	default:
		break;
	}

	clt->clt_fd = fd;
	if (clt->clt_srvbev != NULL)
		bufferevent_free(clt->clt_srvbev);

	clt->clt_srvbev_throttled = 0;
	clt->clt_srvbev = bufferevent_new(clt->clt_fd, server_read,
	    server_write, server_file_error, clt);
	if (clt->clt_srvbev == NULL) {
		errstr = "failed to allocate file buffer event";
		goto fail;
	}

	 
	bufferevent_setwatermark(clt->clt_srvbev, EV_READ, 0,
	    clt->clt_sndbufsiz);

	bufferevent_settimeout(clt->clt_srvbev,
	    srv_conf->timeout.tv_sec, srv_conf->timeout.tv_sec);
	bufferevent_enable(clt->clt_srvbev, EV_READ);
	bufferevent_disable(clt->clt_bev, EV_READ);

 done:
	server_reset_http(clt);
	return (0);
 fail:
	bufferevent_disable(clt->clt_bev, EV_READ|EV_WRITE);
	bufferevent_free(clt->clt_bev);
	clt->clt_bev = NULL;
 abort:
	if (fd != -1)
		close(fd);
	if (errstr == NULL)
		errstr = strerror(errno);
	server_abort_http(clt, code, errstr);
	return (-1);
}