 server_reset_http(struct client *clt)
 {
	struct server		*srv = clt->clt_srv;

	server_log(clt, NULL);

	server_httpdesc_free(clt->clt_descreq);
	server_httpdesc_free(clt->clt_descresp);
	clt->clt_headerlen = 0;
	clt->clt_headersdone = 0;
	clt->clt_done = 0;
	clt->clt_line = 0;
	clt->clt_chunk = 0;
	free(clt->clt_remote_user);
	clt->clt_remote_user = NULL;
	clt->clt_bev->readcb = server_read_http;
	clt->clt_srv_conf = &srv->srv_conf;
	str_match_free(&clt->clt_srv_match);
}