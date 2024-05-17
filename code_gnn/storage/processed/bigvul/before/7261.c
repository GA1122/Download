static int sapi_cgi_activate(TSRMLS_D)
{
	char *path, *doc_root, *server_name;
	uint path_len, doc_root_len, server_name_len;

	 
	if (!SG(request_info).path_translated) {
		return FAILURE;
	}

	if (php_ini_has_per_host_config()) {
		 
		if (fcgi_is_fastcgi()) {
			fcgi_request *request = (fcgi_request*) SG(server_context);

			server_name = FCGI_GETENV(request, "SERVER_NAME");
		} else {
			server_name = getenv("SERVER_NAME");
		}
		 
		if (server_name) {
			server_name_len = strlen(server_name);
			server_name = estrndup(server_name, server_name_len);
			zend_str_tolower(server_name, server_name_len);
			php_ini_activate_per_host_config(server_name, server_name_len + 1 TSRMLS_CC);
			efree(server_name);
		}
	}

	if (php_ini_has_per_dir_config() ||
		(PG(user_ini_filename) && *PG(user_ini_filename))
	) {
		 
		path_len = strlen(SG(request_info).path_translated);

		 
		if (!IS_SLASH(SG(request_info).path_translated[path_len])) {
			path = emalloc(path_len + 2);
			memcpy(path, SG(request_info).path_translated, path_len + 1);
			path_len = zend_dirname(path, path_len);
			path[path_len++] = DEFAULT_SLASH;
		} else {
			path = estrndup(SG(request_info).path_translated, path_len);
			path_len = zend_dirname(path, path_len);
		}
		path[path_len] = 0;

		 
		php_ini_activate_per_dir_config(path, path_len TSRMLS_CC);  

		 
		if (PG(user_ini_filename) && *PG(user_ini_filename)) {
			if (fcgi_is_fastcgi()) {
				fcgi_request *request = (fcgi_request*) SG(server_context);

				doc_root = FCGI_GETENV(request, "DOCUMENT_ROOT");
			} else {
				doc_root = getenv("DOCUMENT_ROOT");
			}

			 
			if (doc_root) {
				doc_root_len = strlen(doc_root);
				if (doc_root_len > 0 && IS_SLASH(doc_root[doc_root_len - 1])) {
					--doc_root_len;
				}
#ifdef PHP_WIN32
				 
				doc_root = estrndup(doc_root, doc_root_len);
				zend_str_tolower(doc_root, doc_root_len);
#endif
				php_cgi_ini_activate_user_config(path, path_len, doc_root, doc_root_len, doc_root_len - 1 TSRMLS_CC);
				
#ifdef PHP_WIN32
				efree(doc_root);
#endif
			}
		}

		efree(path);
	}

	return SUCCESS;
}