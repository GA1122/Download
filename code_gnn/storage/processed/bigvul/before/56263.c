SAPI_API void sapi_activate(TSRMLS_D)
{
	zend_llist_init(&SG(sapi_headers).headers, sizeof(sapi_header_struct), (void (*)(void *)) sapi_free_header, 0);
	SG(sapi_headers).send_default_content_type = 1;

	 
	SG(sapi_headers).http_status_line = NULL;
	SG(sapi_headers).mimetype = NULL;
	SG(headers_sent) = 0;
	SG(callback_run) = 0;
	SG(callback_func) = NULL;
	SG(read_post_bytes) = 0;
	SG(request_info).post_data = NULL;
	SG(request_info).raw_post_data = NULL;
	SG(request_info).current_user = NULL;
	SG(request_info).current_user_length = 0;
	SG(request_info).no_headers = 0;
	SG(request_info).post_entry = NULL;
	SG(request_info).proto_num = 1000;  
	SG(global_request_time) = 0;

	 
	if (SG(request_info).request_method && !strcmp(SG(request_info).request_method, "HEAD")) {
		SG(request_info).headers_only = 1;
	} else {
		SG(request_info).headers_only = 0;
	}
	SG(rfc1867_uploaded_files) = NULL;

	 
	if (SG(server_context)) {
		if (PG(enable_post_data_reading) && SG(request_info).request_method) {
			if (SG(request_info).content_type && !strcmp(SG(request_info).request_method, "POST")) {
				 
				sapi_read_post_data(TSRMLS_C);
			} else {
				 
				SG(request_info).content_type_dup = NULL;
				if (sapi_module.default_post_reader) {
					sapi_module.default_post_reader(TSRMLS_C);
				}
			}
		} else {
			SG(request_info).content_type_dup = NULL;
		}

		 
		SG(request_info).cookie_data = sapi_module.read_cookies(TSRMLS_C);

		if (sapi_module.activate) {
			sapi_module.activate(TSRMLS_C);
		}
	}
	if (sapi_module.input_filter_init) {
		sapi_module.input_filter_init(TSRMLS_C);
	}
}
