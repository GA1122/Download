server_httpdesc_init(struct client *clt)
{
	struct http_descriptor	*desc;

	if ((desc = calloc(1, sizeof(*desc))) == NULL)
		return (-1);
	RB_INIT(&desc->http_headers);
	clt->clt_descreq = desc;

	if ((desc = calloc(1, sizeof(*desc))) == NULL) {
		 
		return (-1);
	}
	RB_INIT(&desc->http_headers);
	clt->clt_descresp = desc;

	return (0);
}
