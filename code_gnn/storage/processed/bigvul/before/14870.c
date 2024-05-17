PHP_METHOD(SoapClient, SoapClient)
{

	zval *wsdl, *options = NULL;
	int  soap_version = SOAP_1_1;
	php_stream_context *context = NULL;
	long cache_wsdl;
	sdlPtr sdl = NULL;
	HashTable *typemap_ht = NULL;

	SOAP_CLIENT_BEGIN_CODE();

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "z|a", &wsdl, &options) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Invalid parameters");
	}

	if (Z_TYPE_P(wsdl) != IS_STRING && Z_TYPE_P(wsdl) != IS_NULL) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "$wsdl must be string or null");
	}

	cache_wsdl = SOAP_GLOBAL(cache_enabled) ? SOAP_GLOBAL(cache_mode) : 0;

	if (options != NULL) {
		HashTable *ht = Z_ARRVAL_P(options);
		zval **tmp;

		if (Z_TYPE_P(wsdl) == IS_NULL) {
			 
			if (zend_hash_find(ht, "uri", sizeof("uri"), (void**)&tmp) == SUCCESS &&
			    Z_TYPE_PP(tmp) == IS_STRING) {
				add_property_stringl(this_ptr, "uri", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
			} else {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "'uri' option is required in nonWSDL mode");
			}

			if (zend_hash_find(ht, "style", sizeof("style"), (void**)&tmp) == SUCCESS &&
					Z_TYPE_PP(tmp) == IS_LONG &&
					(Z_LVAL_PP(tmp) == SOAP_RPC || Z_LVAL_PP(tmp) == SOAP_DOCUMENT)) {
				add_property_long(this_ptr, "style", Z_LVAL_PP(tmp));
			}

			if (zend_hash_find(ht, "use", sizeof("use"), (void**)&tmp) == SUCCESS &&
					Z_TYPE_PP(tmp) == IS_LONG &&
					(Z_LVAL_PP(tmp) == SOAP_LITERAL || Z_LVAL_PP(tmp) == SOAP_ENCODED)) {
				add_property_long(this_ptr, "use", Z_LVAL_PP(tmp));
			}
		}

		if (zend_hash_find(ht, "stream_context", sizeof("stream_context"), (void**)&tmp) == SUCCESS &&
				Z_TYPE_PP(tmp) == IS_RESOURCE) {
			context = php_stream_context_from_zval(*tmp, 1);
			zend_list_addref(context->rsrc_id);
		}

		if (zend_hash_find(ht, "location", sizeof("location"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_STRING) {
			add_property_stringl(this_ptr, "location", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
		} else if (Z_TYPE_P(wsdl) == IS_NULL) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "'location' option is required in nonWSDL mode");
		}

		if (zend_hash_find(ht, "soap_version", sizeof("soap_version"), (void**)&tmp) == SUCCESS) {
			if (Z_TYPE_PP(tmp) == IS_LONG ||
			    (Z_LVAL_PP(tmp) == SOAP_1_1 && Z_LVAL_PP(tmp) == SOAP_1_2)) {
				soap_version = Z_LVAL_PP(tmp);
			}
		}
		if (zend_hash_find(ht, "login", sizeof("login"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_STRING) {
			add_property_stringl(this_ptr, "_login", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
			if (zend_hash_find(ht, "password", sizeof("password"), (void**)&tmp) == SUCCESS &&
			    Z_TYPE_PP(tmp) == IS_STRING) {
				add_property_stringl(this_ptr, "_password", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
			}
			if (zend_hash_find(ht, "authentication", sizeof("authentication"), (void**)&tmp) == SUCCESS &&
			    Z_TYPE_PP(tmp) == IS_LONG &&
			    Z_LVAL_PP(tmp) == SOAP_AUTHENTICATION_DIGEST) {
				add_property_null(this_ptr, "_digest");
			}
		}
		if (zend_hash_find(ht, "proxy_host", sizeof("proxy_host"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_STRING) {
			add_property_stringl(this_ptr, "_proxy_host", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
			if (zend_hash_find(ht, "proxy_port", sizeof("proxy_port"), (void**)&tmp) == SUCCESS) {
				convert_to_long(*tmp);
				add_property_long(this_ptr, "_proxy_port", Z_LVAL_PP(tmp));
			}
			if (zend_hash_find(ht, "proxy_login", sizeof("proxy_login"), (void**)&tmp) == SUCCESS &&
			    Z_TYPE_PP(tmp) == IS_STRING) {
				add_property_stringl(this_ptr, "_proxy_login", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
				if (zend_hash_find(ht, "proxy_password", sizeof("proxy_password"), (void**)&tmp) == SUCCESS &&
				    Z_TYPE_PP(tmp) == IS_STRING) {
					add_property_stringl(this_ptr, "_proxy_password", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
				}
			}
		}
		if (zend_hash_find(ht, "local_cert", sizeof("local_cert"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_STRING) {
		  if (!context) {
  			context = php_stream_context_alloc(TSRMLS_C);
		  }
 			php_stream_context_set_option(context, "ssl", "local_cert", *tmp);
			if (zend_hash_find(ht, "passphrase", sizeof("passphrase"), (void**)&tmp) == SUCCESS &&
			    Z_TYPE_PP(tmp) == IS_STRING) {
				php_stream_context_set_option(context, "ssl", "passphrase", *tmp);
			}
		}
		if (zend_hash_find(ht, "trace", sizeof("trace"), (void**)&tmp) == SUCCESS &&
		    (Z_TYPE_PP(tmp) == IS_BOOL || Z_TYPE_PP(tmp) == IS_LONG) &&
				Z_LVAL_PP(tmp) == 1) {
			add_property_long(this_ptr, "trace", 1);
		}

		if (zend_hash_find(ht, "exceptions", sizeof("exceptions"), (void**)&tmp) == SUCCESS &&
		    (Z_TYPE_PP(tmp) == IS_BOOL || Z_TYPE_PP(tmp) == IS_LONG) &&
				Z_LVAL_PP(tmp) == 0) {
			add_property_bool(this_ptr, "_exceptions", 0);
		}

		if (zend_hash_find(ht, "compression", sizeof("compression"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_LONG &&
	      zend_hash_exists(EG(function_table), "gzinflate", sizeof("gzinflate")) &&
	      zend_hash_exists(EG(function_table), "gzdeflate", sizeof("gzdeflate")) &&
	      zend_hash_exists(EG(function_table), "gzuncompress", sizeof("gzuncompress")) &&
	      zend_hash_exists(EG(function_table), "gzcompress", sizeof("gzcompress")) &&
	      zend_hash_exists(EG(function_table), "gzencode", sizeof("gzencode"))) {
			add_property_long(this_ptr, "compression", Z_LVAL_PP(tmp));
		}
		if (zend_hash_find(ht, "encoding", sizeof("encoding"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_STRING) {
			xmlCharEncodingHandlerPtr encoding;
		
			encoding = xmlFindCharEncodingHandler(Z_STRVAL_PP(tmp));
			if (encoding == NULL) {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Invalid 'encoding' option - '%s'", Z_STRVAL_PP(tmp));
			} else {
				xmlCharEncCloseFunc(encoding);
				add_property_stringl(this_ptr, "_encoding", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);			
			}
		}
		if (zend_hash_find(ht, "classmap", sizeof("classmap"), (void**)&tmp) == SUCCESS &&
			Z_TYPE_PP(tmp) == IS_ARRAY) {
			zval *class_map;

			MAKE_STD_ZVAL(class_map);
			MAKE_COPY_ZVAL(tmp, class_map);
			Z_DELREF_P(class_map);

			add_property_zval(this_ptr, "_classmap", class_map);
		}

		if (zend_hash_find(ht, "typemap", sizeof("typemap"), (void**)&tmp) == SUCCESS &&
			Z_TYPE_PP(tmp) == IS_ARRAY &&
			zend_hash_num_elements(Z_ARRVAL_PP(tmp)) > 0) {
			typemap_ht = Z_ARRVAL_PP(tmp);
		}

		if (zend_hash_find(ht, "features", sizeof("features"), (void**)&tmp) == SUCCESS &&
			Z_TYPE_PP(tmp) == IS_LONG) {
			add_property_long(this_ptr, "_features", Z_LVAL_PP(tmp));
	    }

		if (zend_hash_find(ht, "connection_timeout", sizeof("connection_timeout"), (void**)&tmp) == SUCCESS) {
			convert_to_long(*tmp);
			if (Z_LVAL_PP(tmp) > 0) {
				add_property_long(this_ptr, "_connection_timeout", Z_LVAL_PP(tmp));
			}
		}

		if (context) {
			add_property_resource(this_ptr, "_stream_context", context->rsrc_id);
		}
	
		if (zend_hash_find(ht, "cache_wsdl", sizeof("cache_wsdl"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_LONG) {
			cache_wsdl = Z_LVAL_PP(tmp);
		}

		if (zend_hash_find(ht, "user_agent", sizeof("user_agent"), (void**)&tmp) == SUCCESS &&
		    Z_TYPE_PP(tmp) == IS_STRING) {
			add_property_stringl(this_ptr, "_user_agent", Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), 1);
		}
		
		if (zend_hash_find(ht, "keep_alive", sizeof("keep_alive"), (void**)&tmp) == SUCCESS &&
				(Z_TYPE_PP(tmp) == IS_BOOL || Z_TYPE_PP(tmp) == IS_LONG) && Z_LVAL_PP(tmp) == 0) {
			add_property_long(this_ptr, "_keep_alive", 0);
		}
	} else if (Z_TYPE_P(wsdl) == IS_NULL) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "'location' and 'uri' options are required in nonWSDL mode");
	}

	add_property_long(this_ptr, "_soap_version", soap_version);

	if (Z_TYPE_P(wsdl) != IS_NULL) {
		int    old_soap_version, ret;

		old_soap_version = SOAP_GLOBAL(soap_version);
		SOAP_GLOBAL(soap_version) = soap_version;

		sdl = get_sdl(this_ptr, Z_STRVAL_P(wsdl), cache_wsdl TSRMLS_CC);
		ret = zend_list_insert(sdl, le_sdl TSRMLS_CC);

		add_property_resource(this_ptr, "sdl", ret);

		SOAP_GLOBAL(soap_version) = old_soap_version;
	}

	if (typemap_ht) {
		HashTable *typemap = soap_create_typemap(sdl, typemap_ht TSRMLS_CC);
		if (typemap) {
			int ret;

			ret = zend_list_insert(typemap, le_typemap TSRMLS_CC);
			add_property_resource(this_ptr, "typemap", ret);
		}
	}
	SOAP_CLIENT_END_CODE();
}
