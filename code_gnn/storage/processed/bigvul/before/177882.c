 static int php_handler(request_rec *r)
 {
 	php_struct * volatile ctx;
 	void *conf;
 	apr_bucket_brigade * volatile brigade;
 	apr_bucket *bucket;
 	apr_status_t rv;
 	request_rec * volatile parent_req = NULL;
 	TSRMLS_FETCH();
 
 #define PHPAP_INI_OFF php_apache_ini_dtor(r, parent_req TSRMLS_CC);
 
 	conf = ap_get_module_config(r->per_dir_config, &php5_module);
 
 	 
 	ctx = SG(server_context);
 	if (ctx == NULL || (ctx && ctx->request_processed && !strcmp(r->protocol, "INCLUDED"))) {
 normal:
 		ctx = SG(server_context) = apr_pcalloc(r->pool, sizeof(*ctx));
 		 
 		apr_pool_cleanup_register(r->pool, (void *)&SG(server_context), php_server_context_cleanup, apr_pool_cleanup_null);
 		ctx->r = r;
 		ctx = NULL;  
 	} else {
 		parent_req = ctx->r;
 		ctx->r = r;
 	}
 	apply_config(conf);
 
 	if (strcmp(r->handler, PHP_MAGIC_TYPE) && strcmp(r->handler, PHP_SOURCE_MAGIC_TYPE) && strcmp(r->handler, PHP_SCRIPT)) {
 		 
 		if (!AP2(xbithack) || strcmp(r->handler, "text/html") || !(r->finfo.protection & APR_UEXECUTE)) {
 			PHPAP_INI_OFF;
 			return DECLINED;
 		}
 	}
 
 	 
 	if (r->used_path_info == AP_REQ_REJECT_PATH_INFO
 		&& r->path_info && r->path_info[0]) {
 		PHPAP_INI_OFF;
 		return HTTP_NOT_FOUND;
 	}
 
 	 
 	if (!AP2(engine)) {
 		PHPAP_INI_OFF;
 		return DECLINED;
 	}
 
 	if (r->finfo.filetype == 0) {
 		php_apache_sapi_log_message_ex("script '%s' not found or unable to stat", r TSRMLS_CC);
 		PHPAP_INI_OFF;
 		return HTTP_NOT_FOUND;
 	}
 	if (r->finfo.filetype == APR_DIR) {
 		php_apache_sapi_log_message_ex("attempt to invoke directory '%s' as script", r TSRMLS_CC);
 		PHPAP_INI_OFF;
 		return HTTP_FORBIDDEN;
 	}
 
 	 
 	if (r->main == NULL ||
 		 
 		r->subprocess_env != r->main->subprocess_env
 	) {
 		 
 		ap_add_common_vars(r);
 		ap_add_cgi_vars(r);
 	}
 
 zend_first_try {
 
 	if (ctx == NULL) {
 		brigade = apr_brigade_create(r->pool, r->connection->bucket_alloc);
 		ctx = SG(server_context);
 		ctx->brigade = brigade;
 
 		if (php_apache_request_ctor(r, ctx TSRMLS_CC)!=SUCCESS) {
 			zend_bailout();
 		}
 	} else {
 		if (!parent_req) {
 			parent_req = ctx->r;
 		}
 		if (parent_req && parent_req->handler &&
 				strcmp(parent_req->handler, PHP_MAGIC_TYPE) &&
 				strcmp(parent_req->handler, PHP_SOURCE_MAGIC_TYPE) &&
 				strcmp(parent_req->handler, PHP_SCRIPT)) {
 			if (php_apache_request_ctor(r, ctx TSRMLS_CC)!=SUCCESS) {
 				zend_bailout();
 			}
 		}
 
 		 
 		if (parent_req && parent_req->status != HTTP_OK && parent_req->status != 413 && strcmp(r->protocol, "INCLUDED")) {
 			parent_req = NULL;
 			goto normal;
 		}
 		ctx->r = r;
 		brigade = ctx->brigade;
 	}
 
 	if (AP2(last_modified)) {
 		ap_update_mtime(r, r->finfo.mtime);
 		ap_set_last_modified(r);
 	}
 
 	 
 	if (strncmp(r->handler, PHP_SOURCE_MAGIC_TYPE, sizeof(PHP_SOURCE_MAGIC_TYPE) - 1) == 0) {
 		zend_syntax_highlighter_ini syntax_highlighter_ini;
 		php_get_highlight_struct(&syntax_highlighter_ini);
 		highlight_file((char *)r->filename, &syntax_highlighter_ini TSRMLS_CC);
 	} else {
 		zend_file_handle zfd;
 
 		zfd.type = ZEND_HANDLE_FILENAME;
 		zfd.filename = (char *) r->filename;
 		zfd.free_filename = 0;
 		zfd.opened_path = NULL;
 
 		if (!parent_req) {
 			php_execute_script(&zfd TSRMLS_CC);
 		} else {
 			zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC, NULL, 1, &zfd);
 		}
 
 		apr_table_set(r->notes, "mod_php_memory_usage",
 			apr_psprintf(ctx->r->pool, "%" APR_SIZE_T_FMT, zend_memory_peak_usage(1 TSRMLS_CC)));
 	}
 
 } zend_end_try();
 
 	if (!parent_req) {
 		php_apache_request_dtor(r TSRMLS_CC);
 		ctx->request_processed = 1;
 		bucket = apr_bucket_eos_create(r->connection->bucket_alloc);
 		APR_BRIGADE_INSERT_TAIL(brigade, bucket);
 
 		rv = ap_pass_brigade(r->output_filters, brigade);
 		if (rv != APR_SUCCESS || r->connection->aborted) {
 zend_first_try {
 			php_handle_aborted_connection();
  } zend_end_try();
                 }
                 apr_brigade_cleanup(brigade);
//                apr_pool_cleanup_run(r->pool, (void *)&SG(server_context), php_server_context_cleanup);
         } else {
                 ctx->r = parent_req;
         }
 	return OK;
 }