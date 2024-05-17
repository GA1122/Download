 server_partial_file_request(struct httpd *env, struct client *clt, char *path,
     struct stat *st, char *range_str)
 {
 	struct server_config	*srv_conf = clt->clt_srv_conf;
  	struct http_descriptor	*resp = clt->clt_descresp;
  	struct http_descriptor	*desc = clt->clt_descreq;
  	struct media_type	*media, multipart_media;
// 	struct range_data	*r = &clt->clt_ranges;
  	struct range		*range;
	struct evbuffer		*evb = NULL;
	size_t			 content_length;
// 	size_t			 content_length = 0;
  	int			 code = 500, fd = -1, i, nranges, ret;
	uint32_t		 boundary;
  	char			 content_range[64];
  	const char		*errstr = NULL;
  
  	 
  	if (desc->http_method != HTTP_METHOD_GET)
  		return server_file_request(env, clt, path, st);
  
	if ((range = parse_range(range_str, st->st_size, &nranges)) == NULL) {
// 	if ((nranges = parse_ranges(clt, range_str, st->st_size)) < 1) {
  		code = 416;
  		(void)snprintf(content_range, sizeof(content_range),
  		    "bytes */%lld", st->st_size);
 		errstr = content_range;
 		goto abort;
 	}
 
 	 
 	if ((fd = open(path, O_RDONLY)) == -1)
  		goto abort;
  
  	media = media_find_config(env, srv_conf, path);
	if ((evb = evbuffer_new()) == NULL) {
		errstr = "failed to allocate file buffer";
		goto abort;
	}
// 	r->range_media = media;
  
  	if (nranges == 1) {
// 		range = &r->range[0];
  		(void)snprintf(content_range, sizeof(content_range),
  		    "bytes %lld-%lld/%lld", range->start, range->end,
  		    st->st_size);
  		if (kv_add(&resp->http_headers, "Content-Range",
  		    content_range) == NULL)
  			goto abort;
  
		content_length = range->end - range->start + 1;
		if (buffer_add_range(fd, evb, range) == 0)
			goto abort;
// 		range = &r->range[0];
// 		content_length += range->end - range->start + 1;
  	} else {
		content_length = 0;
		boundary = arc4random();
 		 
		while (nranges--) {
			if ((i = evbuffer_add_printf(evb, "\r\n--%ud\r\n",
			    boundary)) == -1)
				goto abort;
// 		arc4random_buf(&clt->clt_boundary, sizeof(clt->clt_boundary));
  
			content_length += i;
			if ((i = evbuffer_add_printf(evb,
			    "Content-Type: %s/%s\r\n",
			    media->media_type, media->media_subtype)) == -1)
				goto abort;
// 		 
// 		for (i = 0; i < nranges; i++) {
// 			range = &r->range[i];
  
			content_length += i;
			if ((i = evbuffer_add_printf(evb,
// 			 
// 			if ((ret = snprintf(NULL, 0,
// 			    "\r\n--%llu\r\n"
// 			    "Content-Type: %s/%s\r\n"
  			    "Content-Range: bytes %lld-%lld/%lld\r\n\r\n",
			    range->start, range->end, st->st_size)) == -1)
// 			    clt->clt_boundary,
// 			    media->media_type, media->media_subtype,
// 			    range->start, range->end, st->st_size)) < 0)
  				goto abort;
  
			content_length += i;
			if (buffer_add_range(fd, evb, range) == 0)
				goto abort;
// 			 
// 			content_length += ret + range->end - range->start + 1;
  
			content_length += range->end - range->start + 1;
			range++;
  		}
		if ((i = evbuffer_add_printf(evb, "\r\n--%ud--\r\n",
		    boundary)) == -1)
// 		if ((ret = snprintf(NULL, 0, "\r\n--%llu--\r\n",
// 		    clt->clt_boundary)) < 0)
  			goto abort;
		content_length += i;
// 		content_length += ret;
  
  		 
  		(void)strlcpy(multipart_media.media_type, "multipart",
  		    sizeof(multipart_media.media_type));
  		(void)snprintf(multipart_media.media_subtype,
  		    sizeof(multipart_media.media_subtype),
		    "byteranges; boundary=%ud", boundary);
// 		    "byteranges; boundary=%llu", clt->clt_boundary);
  		media = &multipart_media;
  	}
  
	close(fd);
	fd = -1;
// 	 
// 	r->range_toread = TOREAD_HTTP_RANGE;
  
  	ret = server_response_http(clt, 206, media, content_length,
  	    MINIMUM(time(NULL), st->st_mtim.tv_sec));
 	switch (ret) {
 	case -1:
  		goto fail;
  	case 0:
  		 
// 		close(fd);
  		goto done;
  	default:
  		break;
  	}
  
	if (server_bufferevent_write_buffer(clt, evb) == -1)
// 	clt->clt_fd = fd;
// 	if (clt->clt_srvbev != NULL)
// 		bufferevent_free(clt->clt_srvbev);
// 
// 	clt->clt_srvbev_throttled = 0;
// 	clt->clt_srvbev = bufferevent_new(clt->clt_fd, server_read_httprange,
// 	    server_write, server_file_error, clt);
// 	if (clt->clt_srvbev == NULL) {
// 		errstr = "failed to allocate file buffer event";
  		goto fail;
// 	}
  
	bufferevent_enable(clt->clt_bev, EV_READ|EV_WRITE);
	if (clt->clt_persist)
		clt->clt_toread = TOREAD_HTTP_HEADER;
	else
		clt->clt_toread = TOREAD_HTTP_NONE;
	clt->clt_done = 0;
// 	 
// 	bufferevent_setwatermark(clt->clt_srvbev, EV_READ, 0,
// 	    clt->clt_sndbufsiz);
// 
// 	bufferevent_settimeout(clt->clt_srvbev,
// 	    srv_conf->timeout.tv_sec, srv_conf->timeout.tv_sec);
// 	bufferevent_enable(clt->clt_srvbev, EV_READ);
// 	bufferevent_disable(clt->clt_bev, EV_READ);
  
   done:
	evbuffer_free(evb);
  	server_reset_http(clt);
  	return (0);
   fail:
  	bufferevent_disable(clt->clt_bev, EV_READ|EV_WRITE);
  	bufferevent_free(clt->clt_bev);
  	clt->clt_bev = NULL;
   abort:
	if (evb != NULL)
		evbuffer_free(evb);
  	if (fd != -1)
  		close(fd);
  	if (errstr == NULL)
 		errstr = strerror(errno);
 	server_abort_http(clt, code, errstr);
 	return (-1);
 }