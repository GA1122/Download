PHP_METHOD(SoapServer, handle)
{
	int soap_version, old_soap_version;
	sdlPtr old_sdl = NULL;
	soapServicePtr service;
	xmlDocPtr doc_request=NULL, doc_return;
	zval function_name, **params, *soap_obj, *retval;
	char *fn_name, cont_len[30];
	int num_params = 0, size, i, call_status = 0;
	xmlChar *buf;
	HashTable *function_table;
	soapHeader *soap_headers = NULL;
	sdlFunctionPtr function;
	char *arg = NULL;
	int arg_len = 0;
	xmlCharEncodingHandlerPtr old_encoding;
	HashTable *old_class_map, *old_typemap;
	int old_features;

	SOAP_SERVER_BEGIN_CODE();

	FETCH_THIS_SERVICE(service);
	SOAP_GLOBAL(soap_version) = service->version;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &arg, &arg_len) == FAILURE) {
		return;
	}

	if (SG(request_info).request_method &&
	    strcmp(SG(request_info).request_method, "GET") == 0 &&
	    SG(request_info).query_string &&
	    stricmp(SG(request_info).query_string, "wsdl") == 0) {

		if (service->sdl) {
 
			zval readfile, readfile_ret, *param;

			INIT_ZVAL(readfile);
			INIT_ZVAL(readfile_ret);
			MAKE_STD_ZVAL(param);

			sapi_add_header("Content-Type: text/xml; charset=utf-8", sizeof("Content-Type: text/xml; charset=utf-8")-1, 1);
			ZVAL_STRING(param, service->sdl->source, 1);
			ZVAL_STRING(&readfile, "readfile", 1);
			if (call_user_function(EG(function_table), NULL, &readfile, &readfile_ret, 1, &param  TSRMLS_CC) == FAILURE) {
				soap_server_fault("Server", "Couldn't find WSDL", NULL, NULL, NULL TSRMLS_CC);
			}

			zval_ptr_dtor(&param);
			zval_dtor(&readfile);
			zval_dtor(&readfile_ret);

			SOAP_SERVER_END_CODE();
			return;
		} else {
			soap_server_fault("Server", "WSDL generation is not supported yet", NULL, NULL, NULL TSRMLS_CC);
 
			SOAP_SERVER_END_CODE();
			return;
		}
	}

	ALLOC_INIT_ZVAL(retval);

	if (php_output_start_default(TSRMLS_C) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR,"ob_start failed");
	}

	if (ZEND_NUM_ARGS() == 0) {
		if (SG(request_info).raw_post_data) {
			char *post_data = SG(request_info).raw_post_data;
			int post_data_length = SG(request_info).raw_post_data_length;
			zval **server_vars, **encoding;

			zend_is_auto_global("_SERVER", sizeof("_SERVER")-1 TSRMLS_CC);
			if (zend_hash_find(&EG(symbol_table), "_SERVER", sizeof("_SERVER"), (void **) &server_vars) == SUCCESS &&
			    Z_TYPE_PP(server_vars) == IS_ARRAY &&
			    zend_hash_find(Z_ARRVAL_PP(server_vars), "HTTP_CONTENT_ENCODING", sizeof("HTTP_CONTENT_ENCODING"), (void **) &encoding)==SUCCESS &&
			    Z_TYPE_PP(encoding) == IS_STRING) {
				zval func;
				zval retval;
				zval param;
				zval *params[1];

				if ((strcmp(Z_STRVAL_PP(encoding),"gzip") == 0 ||
				     strcmp(Z_STRVAL_PP(encoding),"x-gzip") == 0) &&
				    zend_hash_exists(EG(function_table), "gzinflate", sizeof("gzinflate"))) {
					ZVAL_STRING(&func, "gzinflate", 0);
					params[0] = &param;
					ZVAL_STRINGL(params[0], post_data+10, post_data_length-10, 0);
					INIT_PZVAL(params[0]);
				} else if (strcmp(Z_STRVAL_PP(encoding),"deflate") == 0 &&
		           zend_hash_exists(EG(function_table), "gzuncompress", sizeof("gzuncompress"))) {
					ZVAL_STRING(&func, "gzuncompress", 0);
					params[0] = &param;
					ZVAL_STRINGL(params[0], post_data, post_data_length, 0);
					INIT_PZVAL(params[0]);
				} else {
					php_error_docref(NULL TSRMLS_CC, E_WARNING,"Request is compressed with unknown compression '%s'",Z_STRVAL_PP(encoding));
					return;
				}
				if (call_user_function(CG(function_table), (zval**)NULL, &func, &retval, 1, params TSRMLS_CC) == SUCCESS &&
				    Z_TYPE(retval) == IS_STRING) {
					doc_request = soap_xmlParseMemory(Z_STRVAL(retval),Z_STRLEN(retval));
					zval_dtor(&retval);
				} else {
					php_error_docref(NULL TSRMLS_CC, E_WARNING,"Can't uncompress compressed request");
					return;
				}
			} else {
				doc_request = soap_xmlParseMemory(post_data, post_data_length);
			}
		} else {
			zval_ptr_dtor(&retval);
			return;
		}
	} else {
		doc_request = soap_xmlParseMemory(arg,arg_len);
	}

	if (doc_request == NULL) {
		soap_server_fault("Client", "Bad Request", NULL, NULL, NULL TSRMLS_CC);
	}
	if (xmlGetIntSubset(doc_request) != NULL) {
		xmlNodePtr env = get_node(doc_request->children,"Envelope");
		if (env && env->ns) {
			if (strcmp((char*)env->ns->href, SOAP_1_1_ENV_NAMESPACE) == 0) {
				SOAP_GLOBAL(soap_version) = SOAP_1_1;
			} else if (strcmp((char*)env->ns->href,SOAP_1_2_ENV_NAMESPACE) == 0) {
				SOAP_GLOBAL(soap_version) = SOAP_1_2;
			}
		}
		xmlFreeDoc(doc_request);
		soap_server_fault("Server", "DTD are not supported by SOAP", NULL, NULL, NULL TSRMLS_CC);
	}

	old_sdl = SOAP_GLOBAL(sdl);
	SOAP_GLOBAL(sdl) = service->sdl;
	old_encoding = SOAP_GLOBAL(encoding);
	SOAP_GLOBAL(encoding) = service->encoding;
	old_class_map = SOAP_GLOBAL(class_map);
	SOAP_GLOBAL(class_map) = service->class_map;
	old_typemap = SOAP_GLOBAL(typemap);
	SOAP_GLOBAL(typemap) = service->typemap;
	old_features = SOAP_GLOBAL(features);
	SOAP_GLOBAL(features) = service->features;
	old_soap_version = SOAP_GLOBAL(soap_version);
	function = deserialize_function_call(service->sdl, doc_request, service->actor, &function_name, &num_params, &params, &soap_version, &soap_headers TSRMLS_CC);
	xmlFreeDoc(doc_request);

	if (EG(exception)) {
		php_output_discard(TSRMLS_C);
		if (Z_TYPE_P(EG(exception)) == IS_OBJECT &&
		    instanceof_function(Z_OBJCE_P(EG(exception)), soap_fault_class_entry TSRMLS_CC)) {
			soap_server_fault_ex(function, EG(exception), NULL TSRMLS_CC);
		}
		goto fail;
	}

	service->soap_headers_ptr = &soap_headers;

	soap_obj = NULL;
	if (service->type == SOAP_OBJECT) {
		soap_obj = service->soap_object;
		function_table = &((Z_OBJCE_P(soap_obj))->function_table);
	} else if (service->type == SOAP_CLASS) {
#if HAVE_PHP_SESSION && !defined(COMPILE_DL_SESSION)
		 
		if (service->soap_class.persistance == SOAP_PERSISTENCE_SESSION) {
			zval **tmp_soap;

			if (PS(session_status) != php_session_active &&
			    PS(session_status) != php_session_disabled) {
				php_session_start(TSRMLS_C);
			}

			 
			if (zend_hash_find(Z_ARRVAL_P(PS(http_session_vars)), "_bogus_session_name", sizeof("_bogus_session_name"), (void **) &tmp_soap) == SUCCESS &&
			    Z_TYPE_PP(tmp_soap) == IS_OBJECT &&
			    Z_OBJCE_PP(tmp_soap) == service->soap_class.ce) {
				soap_obj = *tmp_soap;
			}
		}
#endif
		 
		if (soap_obj == NULL) {
			zval *tmp_soap;

			MAKE_STD_ZVAL(tmp_soap);
			object_init_ex(tmp_soap, service->soap_class.ce);

			 
			if (zend_hash_exists(&Z_OBJCE_P(tmp_soap)->function_table, ZEND_CONSTRUCTOR_FUNC_NAME, sizeof(ZEND_CONSTRUCTOR_FUNC_NAME))) {
				zval c_ret, constructor;

				INIT_ZVAL(c_ret);
				INIT_ZVAL(constructor);

				ZVAL_STRING(&constructor, ZEND_CONSTRUCTOR_FUNC_NAME, 1);
				if (call_user_function(NULL, &tmp_soap, &constructor, &c_ret, service->soap_class.argc, service->soap_class.argv TSRMLS_CC) == FAILURE) {
					php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling constructor");
				}
				if (EG(exception)) {
					php_output_discard(TSRMLS_C);
					if (Z_TYPE_P(EG(exception)) == IS_OBJECT &&
					    instanceof_function(Z_OBJCE_P(EG(exception)), soap_fault_class_entry TSRMLS_CC)) {
						soap_server_fault_ex(function, EG(exception), NULL TSRMLS_CC);
					}
					zval_dtor(&constructor);
					zval_dtor(&c_ret);
					zval_ptr_dtor(&tmp_soap);
					goto fail;
				}
				zval_dtor(&constructor);
				zval_dtor(&c_ret);
			} else {
				int class_name_len = strlen(service->soap_class.ce->name);
				char *class_name = emalloc(class_name_len+1);

				memcpy(class_name, service->soap_class.ce->name,class_name_len+1);
				if (zend_hash_exists(&Z_OBJCE_P(tmp_soap)->function_table, php_strtolower(class_name, class_name_len), class_name_len+1)) {
					zval c_ret, constructor;

					INIT_ZVAL(c_ret);
					INIT_ZVAL(constructor);

					ZVAL_STRING(&constructor, service->soap_class.ce->name, 1);
					if (call_user_function(NULL, &tmp_soap, &constructor, &c_ret, service->soap_class.argc, service->soap_class.argv TSRMLS_CC) == FAILURE) {
						php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling constructor");
					}

					if (EG(exception)) {
						php_output_discard(TSRMLS_C);
						if (Z_TYPE_P(EG(exception)) == IS_OBJECT &&
						    instanceof_function(Z_OBJCE_P(EG(exception)), soap_fault_class_entry TSRMLS_CC)) {
							soap_server_fault_ex(function, EG(exception), NULL TSRMLS_CC);
						}
						zval_dtor(&constructor);
						zval_dtor(&c_ret);
						efree(class_name);
						zval_ptr_dtor(&tmp_soap);
						goto fail;
					}

					zval_dtor(&constructor);
					zval_dtor(&c_ret);
				}
				efree(class_name);
			}
#if HAVE_PHP_SESSION && !defined(COMPILE_DL_SESSION)
			 
			if (service->soap_class.persistance == SOAP_PERSISTENCE_SESSION) {
				zval **tmp_soap_pp;
				if (zend_hash_update(Z_ARRVAL_P(PS(http_session_vars)), "_bogus_session_name", sizeof("_bogus_session_name"), &tmp_soap, sizeof(zval *), (void **)&tmp_soap_pp) == SUCCESS) {
					soap_obj = *tmp_soap_pp;
				}
			} else {
				soap_obj = tmp_soap;
			}
#else
			soap_obj = tmp_soap;
#endif

		}
		function_table = &((Z_OBJCE_P(soap_obj))->function_table);
	} else {
		if (service->soap_functions.functions_all == TRUE) {
			function_table = EG(function_table);
		} else {
			function_table = service->soap_functions.ft;
		}
	}

	doc_return = NULL;

	 
	if (soap_headers != NULL) {
		soapHeader *header = soap_headers;
		while (header != NULL) {
			soapHeader *h = header;

			header = header->next;
#if 0
			if (service->sdl && !h->function && !h->hdr) {
				if (h->mustUnderstand) {
					soap_server_fault("MustUnderstand","Header not understood", NULL, NULL, NULL TSRMLS_CC);
				} else {
					continue;
				}
			}
#endif
			fn_name = estrndup(Z_STRVAL(h->function_name),Z_STRLEN(h->function_name));
			if (zend_hash_exists(function_table, php_strtolower(fn_name, Z_STRLEN(h->function_name)), Z_STRLEN(h->function_name) + 1) ||
			    ((service->type == SOAP_CLASS || service->type == SOAP_OBJECT) &&
			     zend_hash_exists(function_table, ZEND_CALL_FUNC_NAME, sizeof(ZEND_CALL_FUNC_NAME)))) {
				if (service->type == SOAP_CLASS || service->type == SOAP_OBJECT) {
					call_status = call_user_function(NULL, &soap_obj, &h->function_name, &h->retval, h->num_params, h->parameters TSRMLS_CC);
				} else {
					call_status = call_user_function(EG(function_table), NULL, &h->function_name, &h->retval, h->num_params, h->parameters TSRMLS_CC);
				}
				if (call_status != SUCCESS) {
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Function '%s' call failed", Z_STRVAL(h->function_name));
					return;
				}
				if (Z_TYPE(h->retval) == IS_OBJECT &&
				    instanceof_function(Z_OBJCE(h->retval), soap_fault_class_entry TSRMLS_CC)) {
					zval *headerfault = NULL, **tmp;

					if (zend_hash_find(Z_OBJPROP(h->retval), "headerfault", sizeof("headerfault"), (void**)&tmp) == SUCCESS &&
					    Z_TYPE_PP(tmp) != IS_NULL) {
						headerfault = *tmp;
					}
					php_output_discard(TSRMLS_C);
					soap_server_fault_ex(function, &h->retval, h TSRMLS_CC);
					efree(fn_name);
					if (service->type == SOAP_CLASS && soap_obj) {zval_ptr_dtor(&soap_obj);}
					goto fail;
				} else if (EG(exception)) {
					php_output_discard(TSRMLS_C);
					if (Z_TYPE_P(EG(exception)) == IS_OBJECT &&
					    instanceof_function(Z_OBJCE_P(EG(exception)), soap_fault_class_entry TSRMLS_CC)) {
						zval *headerfault = NULL, **tmp;

						if (zend_hash_find(Z_OBJPROP_P(EG(exception)), "headerfault", sizeof("headerfault"), (void**)&tmp) == SUCCESS &&
						    Z_TYPE_PP(tmp) != IS_NULL) {
							headerfault = *tmp;
						}
						soap_server_fault_ex(function, EG(exception), h TSRMLS_CC);
					}
					efree(fn_name);
					if (service->type == SOAP_CLASS && soap_obj) {zval_ptr_dtor(&soap_obj);}
					goto fail;
				}
			} else if (h->mustUnderstand) {
				soap_server_fault("MustUnderstand","Header not understood", NULL, NULL, NULL TSRMLS_CC);
			}
			efree(fn_name);
		}
	}

	fn_name = estrndup(Z_STRVAL(function_name),Z_STRLEN(function_name));
	if (zend_hash_exists(function_table, php_strtolower(fn_name, Z_STRLEN(function_name)), Z_STRLEN(function_name) + 1) ||
	    ((service->type == SOAP_CLASS || service->type == SOAP_OBJECT) &&
	     zend_hash_exists(function_table, ZEND_CALL_FUNC_NAME, sizeof(ZEND_CALL_FUNC_NAME)))) {
		if (service->type == SOAP_CLASS || service->type == SOAP_OBJECT) {
			call_status = call_user_function(NULL, &soap_obj, &function_name, retval, num_params, params TSRMLS_CC);
			if (service->type == SOAP_CLASS) {
#if HAVE_PHP_SESSION && !defined(COMPILE_DL_SESSION)
				if (service->soap_class.persistance != SOAP_PERSISTENCE_SESSION) {
					zval_ptr_dtor(&soap_obj);
					soap_obj = NULL;
				}
#else
				zval_ptr_dtor(&soap_obj);
				soap_obj = NULL;
#endif
			}
		} else {
			call_status = call_user_function(EG(function_table), NULL, &function_name, retval, num_params, params TSRMLS_CC);
		}
	} else {
		php_error(E_ERROR, "Function '%s' doesn't exist", Z_STRVAL(function_name));
	}
	efree(fn_name);

	if (EG(exception)) {
		php_output_discard(TSRMLS_C);
		if (Z_TYPE_P(EG(exception)) == IS_OBJECT &&
		    instanceof_function(Z_OBJCE_P(EG(exception)), soap_fault_class_entry TSRMLS_CC)) {
			soap_server_fault_ex(function, EG(exception), NULL TSRMLS_CC);
		}
		if (service->type == SOAP_CLASS) {
#if HAVE_PHP_SESSION && !defined(COMPILE_DL_SESSION)
			if (soap_obj && service->soap_class.persistance != SOAP_PERSISTENCE_SESSION) {
#else
			if (soap_obj) {
#endif
			  zval_ptr_dtor(&soap_obj);
			}
		}
		goto fail;
	}

	if (call_status == SUCCESS) {
		char *response_name;

		if (Z_TYPE_P(retval) == IS_OBJECT &&
		    instanceof_function(Z_OBJCE_P(retval), soap_fault_class_entry TSRMLS_CC)) {
			php_output_discard(TSRMLS_C);
			soap_server_fault_ex(function, retval, NULL TSRMLS_CC);
			goto fail;
		}

		if (function && function->responseName) {
			response_name = estrdup(function->responseName);
		} else {
			response_name = emalloc(Z_STRLEN(function_name) + sizeof("Response"));
			memcpy(response_name,Z_STRVAL(function_name),Z_STRLEN(function_name));
			memcpy(response_name+Z_STRLEN(function_name),"Response",sizeof("Response"));
		}
		doc_return = serialize_response_call(function, response_name, service->uri, retval, soap_headers, soap_version TSRMLS_CC);
		efree(response_name);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Function '%s' call failed", Z_STRVAL(function_name));
		return;
	}

	if (EG(exception)) {
		php_output_discard(TSRMLS_C);
		if (Z_TYPE_P(EG(exception)) == IS_OBJECT &&
		    instanceof_function(Z_OBJCE_P(EG(exception)), soap_fault_class_entry TSRMLS_CC)) {
			soap_server_fault_ex(function, EG(exception), NULL TSRMLS_CC);
		}
		if (service->type == SOAP_CLASS) {
#if HAVE_PHP_SESSION && !defined(COMPILE_DL_SESSION)
			if (soap_obj && service->soap_class.persistance != SOAP_PERSISTENCE_SESSION) {
#else
			if (soap_obj) {
#endif
			  zval_ptr_dtor(&soap_obj);
			}
		}
		goto fail;
	}

	 
	php_output_discard(TSRMLS_C);

	if (doc_return) {
		 
		xmlDocDumpMemory(doc_return, &buf, &size);

		if (size == 0) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Dump memory failed");
		} 	

		if (soap_version == SOAP_1_2) {
			sapi_add_header("Content-Type: application/soap+xml; charset=utf-8", sizeof("Content-Type: application/soap+xml; charset=utf-8")-1, 1);
		} else {
			sapi_add_header("Content-Type: text/xml; charset=utf-8", sizeof("Content-Type: text/xml; charset=utf-8")-1, 1);
		}

		xmlFreeDoc(doc_return);

		if (zend_ini_long("zlib.output_compression", sizeof("zlib.output_compression"), 0)) {
			sapi_add_header("Connection: close", sizeof("Connection: close")-1, 1);
		} else {
			snprintf(cont_len, sizeof(cont_len), "Content-Length: %d", size);
			sapi_add_header(cont_len, strlen(cont_len), 1);
		}
		php_write(buf, size TSRMLS_CC);
		xmlFree(buf);
	} else {
		sapi_add_header("HTTP/1.1 202 Accepted", sizeof("HTTP/1.1 202 Accepted")-1, 1);
		sapi_add_header("Content-Length: 0", sizeof("Content-Length: 0")-1, 1);
	}

fail:
	SOAP_GLOBAL(soap_version) = old_soap_version;
	SOAP_GLOBAL(encoding) = old_encoding;
	SOAP_GLOBAL(sdl) = old_sdl;
	SOAP_GLOBAL(class_map) = old_class_map;
	SOAP_GLOBAL(typemap) = old_typemap;
	SOAP_GLOBAL(features) = old_features;

	 
	zval_ptr_dtor(&retval);
	while (soap_headers != NULL) {
		soapHeader *h = soap_headers;
		int i;

		soap_headers = soap_headers->next;
		if (h->parameters) {
			i = h->num_params;
			while (i > 0) {
				zval_ptr_dtor(&h->parameters[--i]);
			}
			efree(h->parameters);
		}
		zval_dtor(&h->function_name);
		zval_dtor(&h->retval);
		efree(h);
	}
	service->soap_headers_ptr = NULL;

	 
	if (num_params > 0) {
		for (i = 0; i < num_params;i++) {
			zval_ptr_dtor(&params[i]);
		}
		efree(params);
	}
	zval_dtor(&function_name);

	SOAP_SERVER_END_CODE();
}
 


 
PHP_METHOD(SoapServer, fault)
{
	char *code, *string, *actor=NULL, *name=NULL;
	int code_len, string_len, actor_len = 0, name_len = 0;
	zval* details = NULL;
	soapServicePtr service;
	xmlCharEncodingHandlerPtr old_encoding;

	SOAP_SERVER_BEGIN_CODE();
	FETCH_THIS_SERVICE(service);
	old_encoding = SOAP_GLOBAL(encoding);
	SOAP_GLOBAL(encoding) = service->encoding;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|szs",
	    &code, &code_len, &string, &string_len, &actor, &actor_len, &details,
	    &name, &name_len) == FAILURE) {
		return;
	}

	soap_server_fault(code, string, actor, details, name TSRMLS_CC);

	SOAP_GLOBAL(encoding) = old_encoding;
	SOAP_SERVER_END_CODE();
}
 

PHP_METHOD(SoapServer, addSoapHeader)
{
