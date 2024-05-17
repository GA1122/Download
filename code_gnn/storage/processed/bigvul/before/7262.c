static int sapi_cgi_deactivate(TSRMLS_D)
{
	 
	if (SG(sapi_started)) {
		if (fcgi_is_fastcgi()) {
			if (
#ifndef PHP_WIN32
				!parent &&
#endif
				!fcgi_finish_request((fcgi_request*)SG(server_context), 0)) {
				php_handle_aborted_connection();
			}
		} else {
			sapi_cgi_flush(SG(server_context));
		}
	}
	return SUCCESS;
}
