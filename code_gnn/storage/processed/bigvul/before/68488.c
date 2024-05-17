server_file_method(struct client *clt)
{
	struct http_descriptor	*desc = clt->clt_descreq;

	switch (desc->http_method) {
	case HTTP_METHOD_GET:
	case HTTP_METHOD_HEAD:
		return (0);
	default:
		 
		errno = EACCES;
		return (405);
	}
	 
}