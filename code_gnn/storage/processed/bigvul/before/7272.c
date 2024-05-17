static char *sapi_fcgi_getenv(char *name, size_t name_len TSRMLS_DC)
{
	 
	fcgi_request *request = (fcgi_request*) SG(server_context);
	char *ret = fcgi_getenv(request, name, name_len);

	if (ret) return ret;
	 
	return getenv(name);
}
