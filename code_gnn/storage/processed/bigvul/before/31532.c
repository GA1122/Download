rad_create_request(struct rad_handle *h, int code)
{
	int i;

	h->request[POS_CODE] = code;
	h->request[POS_IDENT] = ++h->ident;
	 
	for (i = 0;  i < LEN_AUTH;  i += 2) {
		long r;
		TSRMLS_FETCH();
		r = php_rand(TSRMLS_C);
		h->request[POS_AUTH+i] = (unsigned char) r;
		h->request[POS_AUTH+i+1] = (unsigned char) (r >> 8);
	}
	h->req_len = POS_ATTRS;
	h->request_created = 1;    
	clear_password(h);
	return 0;
}