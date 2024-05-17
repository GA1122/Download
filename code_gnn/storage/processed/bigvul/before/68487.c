server_file_index(struct httpd *env, struct client *clt, struct stat *st)
{
	char			  path[PATH_MAX];
	char			  tmstr[21];
	struct http_descriptor	 *desc = clt->clt_descreq;
	struct server_config	 *srv_conf = clt->clt_srv_conf;
	struct dirent		**namelist, *dp;
	int			  namesize, i, ret, fd = -1, namewidth, skip;
	int			  code = 500;
	struct evbuffer		 *evb = NULL;
	struct media_type	 *media;
	const char		 *stripped, *style;
	char			 *escapeduri, *escapedhtml, *escapedpath;
	struct tm		  tm;
	time_t			  t, dir_mtime;

	if ((ret = server_file_method(clt)) != 0) {
		code = ret;
		goto abort;
	}

	 
	stripped = server_root_strip(desc->http_path, srv_conf->strip);
	if ((size_t)snprintf(path, sizeof(path), "%s%s",
	    srv_conf->root, stripped) >= sizeof(path))
		goto abort;

	 
	if ((fd = open(path, O_RDONLY)) == -1)
		goto abort;

	 
	dir_mtime = MINIMUM(time(NULL), st->st_mtim.tv_sec);

	if ((evb = evbuffer_new()) == NULL)
		goto abort;

	if ((namesize = scandir(path, &namelist, NULL, alphasort)) == -1)
		goto abort;

	 
	skip = 0;

	if ((escapedpath = escape_html(desc->http_path)) == NULL)
		goto fail;

	 
	style = "body { background-color: white; color: black; font-family: "
	    "sans-serif; }\nhr { border: 0; border-bottom: 1px dashed; }\n";
	 
	if (evbuffer_add_printf(evb,
	    "<!DOCTYPE html>\n"
	    "<html>\n"
	    "<head>\n"
	    "<meta http-equiv=\"Content-Type\" content=\"text/html; "
	    "charset=utf-8\"/>\n"
	    "<title>Index of %s</title>\n"
	    "<style type=\"text/css\"><!--\n%s\n--></style>\n"
	    "</head>\n"
	    "<body>\n"
	    "<h1>Index of %s</h1>\n"
	    "<hr>\n<pre>\n",
	    escapedpath, style, escapedpath) == -1)
		skip = 1;

	free(escapedpath);

	for (i = 0; i < namesize; i++) {
		dp = namelist[i];

		if (skip ||
		    fstatat(fd, dp->d_name, st, 0) == -1) {
			free(dp);
			continue;
		}

		t = st->st_mtime;
		localtime_r(&t, &tm);
		strftime(tmstr, sizeof(tmstr), "%d-%h-%Y %R", &tm);
		namewidth = 51 - strlen(dp->d_name);

		if ((escapeduri = url_encode(dp->d_name)) == NULL)
			goto fail;
		if ((escapedhtml = escape_html(dp->d_name)) == NULL)
			goto fail;

		if (dp->d_name[0] == '.' &&
		    !(dp->d_name[1] == '.' && dp->d_name[2] == '\0')) {
			 
		} else if (S_ISDIR(st->st_mode)) {
			namewidth -= 1;  
			if (evbuffer_add_printf(evb,
			    "<a href=\"%s%s/\">%s/</a>%*s%s%20s\n",
			    strchr(escapeduri, ':') != NULL ? "./" : "",
			    escapeduri, escapedhtml,
			    MAXIMUM(namewidth, 0), " ", tmstr, "-") == -1)
				skip = 1;
		} else if (S_ISREG(st->st_mode)) {
			if (evbuffer_add_printf(evb,
			    "<a href=\"%s%s\">%s</a>%*s%s%20llu\n",
			    strchr(escapeduri, ':') != NULL ? "./" : "",
			    escapeduri, escapedhtml,
			    MAXIMUM(namewidth, 0), " ",
			    tmstr, st->st_size) == -1)
				skip = 1;
		}
		free(escapeduri);
		free(escapedhtml);
		free(dp);
	}
	free(namelist);

	if (skip ||
	    evbuffer_add_printf(evb,
	    "</pre>\n<hr>\n</body>\n</html>\n") == -1)
		goto abort;

	close(fd);
	fd = -1;

	media = media_find_config(env, srv_conf, "index.html");
	ret = server_response_http(clt, 200, media, EVBUFFER_LENGTH(evb),
	    dir_mtime);
	switch (ret) {
	case -1:
		goto fail;
	case 0:
		 
		evbuffer_free(evb);
		goto done;
	default:
		break;
	}

	if (server_bufferevent_write_buffer(clt, evb) == -1)
		goto fail;
	evbuffer_free(evb);
	evb = NULL;

	bufferevent_enable(clt->clt_bev, EV_READ|EV_WRITE);
	if (clt->clt_persist)
		clt->clt_toread = TOREAD_HTTP_HEADER;
	else
		clt->clt_toread = TOREAD_HTTP_NONE;
	clt->clt_done = 0;

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
	if (evb != NULL)
		evbuffer_free(evb);
	server_abort_http(clt, code, desc->http_path);
	return (-1);
}