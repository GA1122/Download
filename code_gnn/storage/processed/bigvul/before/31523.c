is_valid_response(struct rad_handle *h, int srv,
    const struct sockaddr_in *from)
{
	MD5_CTX ctx;
	unsigned char md5[16];
	const struct rad_server *srvp;
	int len;

	srvp = &h->servers[srv];

	 
	if (from->sin_family != srvp->addr.sin_family ||
	    from->sin_addr.s_addr != srvp->addr.sin_addr.s_addr ||
	    from->sin_port != srvp->addr.sin_port)
		return 0;

	 
	if (h->resp_len < POS_ATTRS)
		return 0;
	len = h->response[POS_LENGTH] << 8 | h->response[POS_LENGTH+1];
	if (len > h->resp_len)
		return 0;

	 
	MD5Init(&ctx);
	MD5Update(&ctx, &h->response[POS_CODE], POS_AUTH - POS_CODE);
	MD5Update(&ctx, &h->request[POS_AUTH], LEN_AUTH);
	MD5Update(&ctx, &h->response[POS_ATTRS], len - POS_ATTRS);
	MD5Update(&ctx, srvp->secret, strlen(srvp->secret));
	MD5Final(md5, &ctx);
	if (memcmp(&h->response[POS_AUTH], md5, sizeof md5) != 0)
		return 0;

	return 1;
}