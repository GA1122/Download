static int _php_curl_setopt(php_curl *ch, zend_long option, zval *zvalue)  
{
	CURLcode error = CURLE_OK;
	zend_long lval;

	ZVAL_DEREF(zvalue);
	switch (option) {
		 
		case CURLOPT_SSL_VERIFYHOST:
			lval = zval_get_long(zvalue);
			if (lval == 1) {
#if LIBCURL_VERSION_NUM <= 0x071c00  
				php_error_docref(NULL, E_NOTICE, "CURLOPT_SSL_VERIFYHOST with value 1 is deprecated and will be removed as of libcurl 7.28.1. It is recommended to use value 2 instead");
#else
				php_error_docref(NULL, E_NOTICE, "CURLOPT_SSL_VERIFYHOST no longer accepts the value 1, value 2 will be used instead");
				error = curl_easy_setopt(ch->cp, option, 2);
				break;
#endif
			}
		case CURLOPT_AUTOREFERER:
		case CURLOPT_BUFFERSIZE:
		case CURLOPT_CONNECTTIMEOUT:
		case CURLOPT_COOKIESESSION:
		case CURLOPT_CRLF:
		case CURLOPT_DNS_CACHE_TIMEOUT:
		case CURLOPT_DNS_USE_GLOBAL_CACHE:
		case CURLOPT_FAILONERROR:
		case CURLOPT_FILETIME:
		case CURLOPT_FORBID_REUSE:
		case CURLOPT_FRESH_CONNECT:
		case CURLOPT_FTP_USE_EPRT:
		case CURLOPT_FTP_USE_EPSV:
		case CURLOPT_HEADER:
		case CURLOPT_HTTPGET:
		case CURLOPT_HTTPPROXYTUNNEL:
		case CURLOPT_HTTP_VERSION:
		case CURLOPT_INFILESIZE:
		case CURLOPT_LOW_SPEED_LIMIT:
		case CURLOPT_LOW_SPEED_TIME:
		case CURLOPT_MAXCONNECTS:
		case CURLOPT_MAXREDIRS:
		case CURLOPT_NETRC:
		case CURLOPT_NOBODY:
		case CURLOPT_NOPROGRESS:
		case CURLOPT_NOSIGNAL:
		case CURLOPT_PORT:
		case CURLOPT_POST:
		case CURLOPT_PROXYPORT:
		case CURLOPT_PROXYTYPE:
		case CURLOPT_PUT:
		case CURLOPT_RESUME_FROM:
		case CURLOPT_SSLVERSION:
		case CURLOPT_SSL_VERIFYPEER:
		case CURLOPT_TIMECONDITION:
		case CURLOPT_TIMEOUT:
		case CURLOPT_TIMEVALUE:
		case CURLOPT_TRANSFERTEXT:
		case CURLOPT_UNRESTRICTED_AUTH:
		case CURLOPT_UPLOAD:
		case CURLOPT_VERBOSE:
#if LIBCURL_VERSION_NUM >= 0x070a06  
		case CURLOPT_HTTPAUTH:
#endif
#if LIBCURL_VERSION_NUM >= 0x070a07  
		case CURLOPT_FTP_CREATE_MISSING_DIRS:
		case CURLOPT_PROXYAUTH:
#endif
#if LIBCURL_VERSION_NUM >= 0x070a08  
		case CURLOPT_FTP_RESPONSE_TIMEOUT:
		case CURLOPT_IPRESOLVE:
		case CURLOPT_MAXFILESIZE:
#endif
#if LIBCURL_VERSION_NUM >= 0x070b02  
		case CURLOPT_TCP_NODELAY:
#endif
#if LIBCURL_VERSION_NUM >= 0x070c02  
		case CURLOPT_FTPSSLAUTH:
#endif
#if LIBCURL_VERSION_NUM >= 0x070e01  
		case CURLOPT_IGNORE_CONTENT_LENGTH:
#endif
#if LIBCURL_VERSION_NUM >= 0x070f00  
		case CURLOPT_FTP_SKIP_PASV_IP:
#endif
#if LIBCURL_VERSION_NUM >= 0x070f01  
		case CURLOPT_FTP_FILEMETHOD:
#endif
#if LIBCURL_VERSION_NUM >= 0x070f02  
		case CURLOPT_CONNECT_ONLY:
		case CURLOPT_LOCALPORT:
		case CURLOPT_LOCALPORTRANGE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071000  
		case CURLOPT_SSL_SESSIONID_CACHE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071001  
		case CURLOPT_FTP_SSL_CCC:
		case CURLOPT_SSH_AUTH_TYPES:
#endif
#if LIBCURL_VERSION_NUM >= 0x071002  
		case CURLOPT_CONNECTTIMEOUT_MS:
		case CURLOPT_HTTP_CONTENT_DECODING:
		case CURLOPT_HTTP_TRANSFER_DECODING:
		case CURLOPT_TIMEOUT_MS:
#endif
#if LIBCURL_VERSION_NUM >= 0x071004  
		case CURLOPT_NEW_DIRECTORY_PERMS:
		case CURLOPT_NEW_FILE_PERMS:
#endif
#if LIBCURL_VERSION_NUM >= 0x071100  
		case CURLOPT_USE_SSL:
#elif LIBCURL_VERSION_NUM >= 0x070b00  
		case CURLOPT_FTP_SSL:
#endif
#if LIBCURL_VERSION_NUM >= 0x071100  
		case CURLOPT_APPEND:
		case CURLOPT_DIRLISTONLY:
#else
		case CURLOPT_FTPAPPEND:
		case CURLOPT_FTPLISTONLY:
#endif
#if LIBCURL_VERSION_NUM >= 0x071200  
		case CURLOPT_PROXY_TRANSFER_MODE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071300  
		case CURLOPT_ADDRESS_SCOPE:
#endif
#if LIBCURL_VERSION_NUM >  0x071301  
		case CURLOPT_CERTINFO:
#endif
#if LIBCURL_VERSION_NUM >= 0x071304  
		case CURLOPT_PROTOCOLS:
		case CURLOPT_REDIR_PROTOCOLS:
		case CURLOPT_SOCKS5_GSSAPI_NEC:
		case CURLOPT_TFTP_BLKSIZE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071400  
		case CURLOPT_FTP_USE_PRET:
		case CURLOPT_RTSP_CLIENT_CSEQ:
		case CURLOPT_RTSP_REQUEST:
		case CURLOPT_RTSP_SERVER_CSEQ:
#endif
#if LIBCURL_VERSION_NUM >= 0x071500  
		case CURLOPT_WILDCARDMATCH:
#endif
#if LIBCURL_VERSION_NUM >= 0x071504  
		case CURLOPT_TLSAUTH_TYPE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071600  
		case CURLOPT_GSSAPI_DELEGATION:
#endif
#if LIBCURL_VERSION_NUM >= 0x071800  
		case CURLOPT_ACCEPTTIMEOUT_MS:
#endif
#if LIBCURL_VERSION_NUM >= 0x071900  
		case CURLOPT_SSL_OPTIONS:
		case CURLOPT_TCP_KEEPALIVE:
		case CURLOPT_TCP_KEEPIDLE:
		case CURLOPT_TCP_KEEPINTVL:
#endif
#if LIBCURL_VERSION_NUM >= 0x071f00  
		case CURLOPT_SASL_IR:
#endif
#if LIBCURL_VERSION_NUM >= 0x072400  
		case CURLOPT_EXPECT_100_TIMEOUT_MS:
		case CURLOPT_SSL_ENABLE_ALPN:
		case CURLOPT_SSL_ENABLE_NPN:
#endif
#if LIBCURL_VERSION_NUM >= 0x072500  
		case CURLOPT_HEADEROPT:
#endif
#if LIBCURL_VERSION_NUM >= 0x072900  
		case CURLOPT_SSL_VERIFYSTATUS:
#endif
#if LIBCURL_VERSION_NUM >= 0x072a00  
		case CURLOPT_PATH_AS_IS:
		case CURLOPT_SSL_FALSESTART:
#endif
#if LIBCURL_VERSION_NUM >= 0x072b00  
		case CURLOPT_PIPEWAIT:
#endif
#if LIBCURL_VERSION_NUM >= 0x072e00  
		case CURLOPT_STREAM_WEIGHT:
#endif
#if LIBCURL_VERSION_NUM >= 0x073000  
		case CURLOPT_TFTP_NO_OPTIONS:
#endif
#if LIBCURL_VERSION_NUM >= 0x073100  
		case CURLOPT_TCP_FASTOPEN:
#endif
#if CURLOPT_MUTE != 0
		case CURLOPT_MUTE:
#endif
			lval = zval_get_long(zvalue);
#if LIBCURL_VERSION_NUM >= 0x71304
			if ((option == CURLOPT_PROTOCOLS || option == CURLOPT_REDIR_PROTOCOLS) &&
				(PG(open_basedir) && *PG(open_basedir)) && (lval & CURLPROTO_FILE)) {
					php_error_docref(NULL, E_WARNING, "CURLPROTO_FILE cannot be activated when an open_basedir is set");
					return 1;
			}
#endif
# if defined(ZTS)
			if (option == CURLOPT_DNS_USE_GLOBAL_CACHE) {
				php_error_docref(NULL, E_WARNING, "CURLOPT_DNS_USE_GLOBAL_CACHE cannot be activated when thread safety is enabled");
				return 1;
			}
# endif
			error = curl_easy_setopt(ch->cp, option, lval);
			break;
		case CURLOPT_SAFE_UPLOAD:
			lval = zval_get_long(zvalue);
			if (lval == 0) {
				php_error_docref(NULL, E_WARNING, "Disabling safe uploads is no longer supported");
				return FAILURE;
			}
			break;

		 
		case CURLOPT_CAINFO:
		case CURLOPT_CAPATH:
		case CURLOPT_COOKIE:
		case CURLOPT_EGDSOCKET:
		case CURLOPT_INTERFACE:
		case CURLOPT_PROXY:
		case CURLOPT_PROXYUSERPWD:
		case CURLOPT_REFERER:
		case CURLOPT_SSLCERTTYPE:
		case CURLOPT_SSLENGINE:
		case CURLOPT_SSLENGINE_DEFAULT:
		case CURLOPT_SSLKEY:
		case CURLOPT_SSLKEYPASSWD:
		case CURLOPT_SSLKEYTYPE:
		case CURLOPT_SSL_CIPHER_LIST:
		case CURLOPT_USERAGENT:
		case CURLOPT_USERPWD:
#if LIBCURL_VERSION_NUM >= 0x070e01  
		case CURLOPT_COOKIELIST:
#endif
#if LIBCURL_VERSION_NUM >= 0x070f05  
		case CURLOPT_FTP_ALTERNATIVE_TO_USER:
#endif
#if LIBCURL_VERSION_NUM >= 0x071101  
		case CURLOPT_SSH_HOST_PUBLIC_KEY_MD5:
#endif
#if LIBCURL_VERSION_NUM >= 0x071301  
		case CURLOPT_PASSWORD:
		case CURLOPT_PROXYPASSWORD:
		case CURLOPT_PROXYUSERNAME:
		case CURLOPT_USERNAME:
#endif
#if LIBCURL_VERSION_NUM >= 0x071304  
		case CURLOPT_NOPROXY:
		case CURLOPT_SOCKS5_GSSAPI_SERVICE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071400  
		case CURLOPT_MAIL_FROM:
		case CURLOPT_RTSP_STREAM_URI:
		case CURLOPT_RTSP_TRANSPORT:
#endif
#if LIBCURL_VERSION_NUM >= 0x071504  
		case CURLOPT_TLSAUTH_PASSWORD:
		case CURLOPT_TLSAUTH_USERNAME:
#endif
#if LIBCURL_VERSION_NUM >= 0x071506  
		case CURLOPT_ACCEPT_ENCODING:
		case CURLOPT_TRANSFER_ENCODING:
#else
		case CURLOPT_ENCODING:
#endif
#if LIBCURL_VERSION_NUM >= 0x071800  
		case CURLOPT_DNS_SERVERS:
#endif
#if LIBCURL_VERSION_NUM >= 0x071900  
		case CURLOPT_MAIL_AUTH:
#endif
#if LIBCURL_VERSION_NUM >= 0x072200  
		case CURLOPT_LOGIN_OPTIONS:
#endif
#if LIBCURL_VERSION_NUM >= 0x072700  
		case CURLOPT_PINNEDPUBLICKEY:
#endif
#if LIBCURL_VERSION_NUM >= 0x072b00  
		case CURLOPT_PROXY_SERVICE_NAME:
		case CURLOPT_SERVICE_NAME:
#endif
#if LIBCURL_VERSION_NUM >= 0x072d00  
		case CURLOPT_DEFAULT_PROTOCOL:
#endif
		{
			zend_string *str = zval_get_string(zvalue);
			int ret = php_curl_option_str(ch, option, ZSTR_VAL(str), ZSTR_LEN(str), 0);
			zend_string_release(str);
			return ret;
		}

		 
		case CURLOPT_CUSTOMREQUEST:
		case CURLOPT_FTPPORT:
		case CURLOPT_RANGE:
#if LIBCURL_VERSION_NUM >= 0x070d00  
		case CURLOPT_FTP_ACCOUNT:
#endif
#if LIBCURL_VERSION_NUM >= 0x071400  
		case CURLOPT_RTSP_SESSION_ID:
#endif
#if LIBCURL_VERSION_NUM >= 0x072100  
		case CURLOPT_DNS_INTERFACE:
		case CURLOPT_DNS_LOCAL_IP4:
		case CURLOPT_DNS_LOCAL_IP6:
		case CURLOPT_XOAUTH2_BEARER:
#endif
#if LIBCURL_VERSION_NUM >= 0x072800  
		case CURLOPT_UNIX_SOCKET_PATH:
#endif
#if LIBCURL_VERSION_NUM >= 0x071004  
		case CURLOPT_KRBLEVEL:
#else
		case CURLOPT_KRB4LEVEL:
#endif
		{
			if (Z_ISNULL_P(zvalue)) {
				error = curl_easy_setopt(ch->cp, option, NULL);
			} else {
				zend_string *str = zval_get_string(zvalue);
				int ret = php_curl_option_str(ch, option, ZSTR_VAL(str), ZSTR_LEN(str), 0);
				zend_string_release(str);
				return ret;
			}
			break;
		}

		 
		case CURLOPT_PRIVATE:
		{
			zend_string *str = zval_get_string(zvalue);
			int ret = php_curl_option_str(ch, option, ZSTR_VAL(str), ZSTR_LEN(str), 1);
			zend_string_release(str);
			return ret;
		}

		 
		case CURLOPT_URL:
		{
			zend_string *str = zval_get_string(zvalue);
			int ret = php_curl_option_url(ch, ZSTR_VAL(str), ZSTR_LEN(str));
			zend_string_release(str);
			return ret;
		}

		 
		case CURLOPT_FILE:
		case CURLOPT_INFILE:
		case CURLOPT_STDERR:
		case CURLOPT_WRITEHEADER: {
			FILE *fp = NULL;
			php_stream *what = NULL;

			if (Z_TYPE_P(zvalue) != IS_NULL) {
				what = (php_stream *)zend_fetch_resource2_ex(zvalue, "File-Handle", php_file_le_stream(), php_file_le_pstream());
				if (!what) {
					return FAILURE;
				}

				if (FAILURE == php_stream_cast(what, PHP_STREAM_AS_STDIO, (void *) &fp, REPORT_ERRORS)) {
					return FAILURE;
				}

				if (!fp) {
					return FAILURE;
				}
			}

			error = CURLE_OK;
			switch (option) {
				case CURLOPT_FILE:
					if (!what) {
						if (!Z_ISUNDEF(ch->handlers->write->stream)) {
							zval_ptr_dtor(&ch->handlers->write->stream);
							ZVAL_UNDEF(&ch->handlers->write->stream);
						}
						ch->handlers->write->fp = NULL;
						ch->handlers->write->method = PHP_CURL_STDOUT;
					} else if (what->mode[0] != 'r' || what->mode[1] == '+') {
						zval_ptr_dtor(&ch->handlers->write->stream);
						ch->handlers->write->fp = fp;
						ch->handlers->write->method = PHP_CURL_FILE;
						ZVAL_COPY(&ch->handlers->write->stream, zvalue);
					} else {
						php_error_docref(NULL, E_WARNING, "the provided file handle is not writable");
						return FAILURE;
					}
					break;
				case CURLOPT_WRITEHEADER:
					if (!what) {
						if (!Z_ISUNDEF(ch->handlers->write_header->stream)) {
							zval_ptr_dtor(&ch->handlers->write_header->stream);
							ZVAL_UNDEF(&ch->handlers->write_header->stream);
						}
						ch->handlers->write_header->fp = NULL;
						ch->handlers->write_header->method = PHP_CURL_IGNORE;
					} else if (what->mode[0] != 'r' || what->mode[1] == '+') {
						zval_ptr_dtor(&ch->handlers->write_header->stream);
						ch->handlers->write_header->fp = fp;
						ch->handlers->write_header->method = PHP_CURL_FILE;
						ZVAL_COPY(&ch->handlers->write_header->stream, zvalue);;
					} else {
						php_error_docref(NULL, E_WARNING, "the provided file handle is not writable");
						return FAILURE;
					}
					break;
				case CURLOPT_INFILE:
					if (!what) {
						if (!Z_ISUNDEF(ch->handlers->read->stream)) {
							zval_ptr_dtor(&ch->handlers->read->stream);
							ZVAL_UNDEF(&ch->handlers->read->stream);
						}
						ch->handlers->read->fp = NULL;
						ch->handlers->read->res = NULL;
					} else {
						zval_ptr_dtor(&ch->handlers->read->stream);
						ch->handlers->read->fp = fp;
						ch->handlers->read->res = Z_RES_P(zvalue);
						ZVAL_COPY(&ch->handlers->read->stream, zvalue);
					}
					break;
				case CURLOPT_STDERR:
					if (!what) {
						if (!Z_ISUNDEF(ch->handlers->std_err)) {
							zval_ptr_dtor(&ch->handlers->std_err);
							ZVAL_UNDEF(&ch->handlers->std_err);
						}
					} else if (what->mode[0] != 'r' || what->mode[1] == '+') {
						zval_ptr_dtor(&ch->handlers->std_err);
						ZVAL_COPY(&ch->handlers->std_err, zvalue);
					} else {
						php_error_docref(NULL, E_WARNING, "the provided file handle is not writable");
						return FAILURE;
					}
					 
				default:
					error = curl_easy_setopt(ch->cp, option, fp);
					break;
			}
			break;
		}

		 
		case CURLOPT_HTTP200ALIASES:
		case CURLOPT_HTTPHEADER:
		case CURLOPT_POSTQUOTE:
		case CURLOPT_PREQUOTE:
		case CURLOPT_QUOTE:
		case CURLOPT_TELNETOPTIONS:
#if LIBCURL_VERSION_NUM >= 0x071400  
		case CURLOPT_MAIL_RCPT:
#endif
#if LIBCURL_VERSION_NUM >= 0x071503  
		case CURLOPT_RESOLVE:
#endif
#if LIBCURL_VERSION_NUM >= 0x072500  
		case CURLOPT_PROXYHEADER:
#endif
#if LIBCURL_VERSION_NUM >= 0x073100  
		case CURLOPT_CONNECT_TO:
#endif
		{
			zval *current;
			HashTable *ph;
			zend_string *val;
			struct curl_slist *slist = NULL;

			ph = HASH_OF(zvalue);
			if (!ph) {
				char *name = NULL;
				switch (option) {
					case CURLOPT_HTTPHEADER:
						name = "CURLOPT_HTTPHEADER";
						break;
					case CURLOPT_QUOTE:
						name = "CURLOPT_QUOTE";
						break;
					case CURLOPT_HTTP200ALIASES:
						name = "CURLOPT_HTTP200ALIASES";
						break;
					case CURLOPT_POSTQUOTE:
						name = "CURLOPT_POSTQUOTE";
						break;
					case CURLOPT_PREQUOTE:
						name = "CURLOPT_PREQUOTE";
						break;
					case CURLOPT_TELNETOPTIONS:
						name = "CURLOPT_TELNETOPTIONS";
						break;
#if LIBCURL_VERSION_NUM >= 0x071400  
					case CURLOPT_MAIL_RCPT:
						name = "CURLOPT_MAIL_RCPT";
						break;
#endif
#if LIBCURL_VERSION_NUM >= 0x071503  
					case CURLOPT_RESOLVE:
						name = "CURLOPT_RESOLVE";
						break;
#endif
#if LIBCURL_VERSION_NUM >= 0x072500  
					case CURLOPT_PROXYHEADER:
						name = "CURLOPT_PROXYHEADER";
						break;
#endif
#if LIBCURL_VERSION_NUM >= 0x073100  
					case CURLOPT_CONNECT_TO:
						name = "CURLOPT_CONNECT_TO";
						break;
#endif
				}
				php_error_docref(NULL, E_WARNING, "You must pass either an object or an array with the %s argument", name);
				return FAILURE;
			}

			ZEND_HASH_FOREACH_VAL(ph, current) {
				ZVAL_DEREF(current);
				val = zval_get_string(current);
				slist = curl_slist_append(slist, ZSTR_VAL(val));
				zend_string_release(val);
				if (!slist) {
					php_error_docref(NULL, E_WARNING, "Could not build curl_slist");
					return 1;
				}
			} ZEND_HASH_FOREACH_END();

			if (slist) {
				if ((*ch->clone) == 1) {
					zend_hash_index_update_ptr(ch->to_free->slist, option, slist);
				} else {
					zend_hash_next_index_insert_ptr(ch->to_free->slist, slist);
				}
			}

			error = curl_easy_setopt(ch->cp, option, slist);

			break;
		}

		case CURLOPT_BINARYTRANSFER:
			 
			break;

		case CURLOPT_FOLLOWLOCATION:
			lval = zval_get_long(zvalue);
#if LIBCURL_VERSION_NUM < 0x071304
			if (PG(open_basedir) && *PG(open_basedir)) {
				if (lval != 0) {
					php_error_docref(NULL, E_WARNING, "CURLOPT_FOLLOWLOCATION cannot be activated when an open_basedir is set");
					return FAILURE;
				}
			}
#endif
			error = curl_easy_setopt(ch->cp, option, lval);
			break;

		case CURLOPT_HEADERFUNCTION:
			if (!Z_ISUNDEF(ch->handlers->write_header->func_name)) {
				zval_ptr_dtor(&ch->handlers->write_header->func_name);
				ch->handlers->write_header->fci_cache = empty_fcall_info_cache;
			}
			ZVAL_COPY(&ch->handlers->write_header->func_name, zvalue);
			ch->handlers->write_header->method = PHP_CURL_USER;
			break;

		case CURLOPT_POSTFIELDS:
			if (Z_TYPE_P(zvalue) == IS_ARRAY || Z_TYPE_P(zvalue) == IS_OBJECT) {
				zval *current;
				HashTable *postfields;
				zend_string *string_key;
				zend_ulong  num_key;
				struct HttpPost *first = NULL;
				struct HttpPost *last  = NULL;
				CURLFORMcode form_error;

				postfields = HASH_OF(zvalue);
				if (!postfields) {
					php_error_docref(NULL, E_WARNING, "Couldn't get HashTable in CURLOPT_POSTFIELDS");
					return FAILURE;
				}

				ZEND_HASH_FOREACH_KEY_VAL(postfields, num_key, string_key, current) {
					zend_string *postval;
					 
					if (!string_key) {
						string_key = zend_long_to_str(num_key);
					} else {
						zend_string_addref(string_key);
					}

					ZVAL_DEREF(current);
					if (Z_TYPE_P(current) == IS_OBJECT &&
							instanceof_function(Z_OBJCE_P(current), curl_CURLFile_class)) {
						 
						zval *prop, rv;
						char *type = NULL, *filename = NULL;

						prop = zend_read_property(curl_CURLFile_class, current, "name", sizeof("name")-1, 0, &rv);
						if (Z_TYPE_P(prop) != IS_STRING) {
							php_error_docref(NULL, E_WARNING, "Invalid filename for key %s", ZSTR_VAL(string_key));
						} else {
							postval = Z_STR_P(prop);

							if (php_check_open_basedir(ZSTR_VAL(postval))) {
								return 1;
							}

							prop = zend_read_property(curl_CURLFile_class, current, "mime", sizeof("mime")-1, 0, &rv);
							if (Z_TYPE_P(prop) == IS_STRING && Z_STRLEN_P(prop) > 0) {
								type = Z_STRVAL_P(prop);
							}
							prop = zend_read_property(curl_CURLFile_class, current, "postname", sizeof("postname")-1, 0, &rv);
							if (Z_TYPE_P(prop) == IS_STRING && Z_STRLEN_P(prop) > 0) {
								filename = Z_STRVAL_P(prop);
							}
							form_error = curl_formadd(&first, &last,
											CURLFORM_COPYNAME, ZSTR_VAL(string_key),
											CURLFORM_NAMELENGTH, ZSTR_LEN(string_key),
											CURLFORM_FILENAME, filename ? filename : ZSTR_VAL(postval),
											CURLFORM_CONTENTTYPE, type ? type : "application/octet-stream",
											CURLFORM_FILE, ZSTR_VAL(postval),
											CURLFORM_END);
							if (form_error != CURL_FORMADD_OK) {
								 
								error = (CURLcode)form_error;
							}
						}

						zend_string_release(string_key);
						continue;
					}

					postval = zval_get_string(current);

					 
					form_error = curl_formadd(&first, &last,
										 CURLFORM_COPYNAME, ZSTR_VAL(string_key),
										 CURLFORM_NAMELENGTH, ZSTR_LEN(string_key),
										 CURLFORM_COPYCONTENTS, ZSTR_VAL(postval),
										 CURLFORM_CONTENTSLENGTH, ZSTR_LEN(postval),
										 CURLFORM_END);

					if (form_error != CURL_FORMADD_OK) {
						 
						error = (CURLcode)form_error;
					}
					zend_string_release(postval);
					zend_string_release(string_key);
				} ZEND_HASH_FOREACH_END();

				SAVE_CURL_ERROR(ch, error);
				if (error != CURLE_OK) {
					return FAILURE;
				}

				if ((*ch->clone) == 1) {
					zend_llist_clean(&ch->to_free->post);
				}
				zend_llist_add_element(&ch->to_free->post, &first);
				error = curl_easy_setopt(ch->cp, CURLOPT_HTTPPOST, first);
			} else {
#if LIBCURL_VERSION_NUM >= 0x071101
				zend_string *str = zval_get_string(zvalue);
				 
				error = curl_easy_setopt(ch->cp, CURLOPT_POSTFIELDSIZE, ZSTR_LEN(str));
				error = curl_easy_setopt(ch->cp, CURLOPT_COPYPOSTFIELDS, ZSTR_VAL(str));
				zend_string_release(str);
#else
				char *post = NULL;
				zend_string *str = zval_get_string(zvalue);

				post = estrndup(ZSTR_VAL(str), ZSTR_LEN(str));
				zend_llist_add_element(&ch->to_free->str, &post);

				curl_easy_setopt(ch->cp, CURLOPT_POSTFIELDS, post);
				error = curl_easy_setopt(ch->cp, CURLOPT_POSTFIELDSIZE, ZSTR_LEN(str));
				zend_string_release(str);
#endif
			}
			break;

		case CURLOPT_PROGRESSFUNCTION:
			curl_easy_setopt(ch->cp, CURLOPT_PROGRESSFUNCTION,	curl_progress);
			curl_easy_setopt(ch->cp, CURLOPT_PROGRESSDATA, ch);
			if (ch->handlers->progress == NULL) {
				ch->handlers->progress = ecalloc(1, sizeof(php_curl_progress));
			} else if (!Z_ISUNDEF(ch->handlers->progress->func_name)) {
				zval_ptr_dtor(&ch->handlers->progress->func_name);
				ch->handlers->progress->fci_cache = empty_fcall_info_cache;
			}
			ZVAL_COPY(&ch->handlers->progress->func_name, zvalue);
			ch->handlers->progress->method = PHP_CURL_USER;
			break;

		case CURLOPT_READFUNCTION:
			if (!Z_ISUNDEF(ch->handlers->read->func_name)) {
				zval_ptr_dtor(&ch->handlers->read->func_name);
				ch->handlers->read->fci_cache = empty_fcall_info_cache;
			}
			ZVAL_COPY(&ch->handlers->read->func_name, zvalue);
			ch->handlers->read->method = PHP_CURL_USER;
			break;

		case CURLOPT_RETURNTRANSFER:
			lval = zval_get_long(zvalue);
			if (lval) {
				ch->handlers->write->method = PHP_CURL_RETURN;
			} else {
				ch->handlers->write->method = PHP_CURL_STDOUT;
			}
			break;

		case CURLOPT_WRITEFUNCTION:
			if (!Z_ISUNDEF(ch->handlers->write->func_name)) {
				zval_ptr_dtor(&ch->handlers->write->func_name);
				ch->handlers->write->fci_cache = empty_fcall_info_cache;
			}
			ZVAL_COPY(&ch->handlers->write->func_name, zvalue);
			ch->handlers->write->method = PHP_CURL_USER;
			break;

#if LIBCURL_VERSION_NUM >= 0x070f05  
		case CURLOPT_MAX_RECV_SPEED_LARGE:
		case CURLOPT_MAX_SEND_SPEED_LARGE:
			lval = zval_get_long(zvalue);
			error = curl_easy_setopt(ch->cp, option, (curl_off_t)lval);
			break;
#endif

#if LIBCURL_VERSION_NUM >= 0x071301  
		case CURLOPT_POSTREDIR:
			lval = zval_get_long(zvalue);
			error = curl_easy_setopt(ch->cp, CURLOPT_POSTREDIR, lval & CURL_REDIR_POST_ALL);
			break;
#endif

#if CURLOPT_PASSWDFUNCTION != 0
		case CURLOPT_PASSWDFUNCTION:
			zval_ptr_dtor(&ch->handlers->passwd);
			ZVAL_COPY(&ch->handlers->passwd, zvalue);
			error = curl_easy_setopt(ch->cp, CURLOPT_PASSWDFUNCTION, curl_passwd);
			error = curl_easy_setopt(ch->cp, CURLOPT_PASSWDDATA,     (void *) ch);
			break;
#endif

		 
		case CURLOPT_COOKIEFILE:
		case CURLOPT_COOKIEJAR:
		case CURLOPT_RANDOM_FILE:
		case CURLOPT_SSLCERT:
#if LIBCURL_VERSION_NUM >= 0x070b00  
		case CURLOPT_NETRC_FILE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071001  
		case CURLOPT_SSH_PRIVATE_KEYFILE:
		case CURLOPT_SSH_PUBLIC_KEYFILE:
#endif
#if LIBCURL_VERSION_NUM >= 0x071300  
		case CURLOPT_CRLFILE:
		case CURLOPT_ISSUERCERT:
#endif
#if LIBCURL_VERSION_NUM >= 0x071306  
		case CURLOPT_SSH_KNOWNHOSTS:
#endif
		{
			zend_string *str = zval_get_string(zvalue);
			int ret;

			if (ZSTR_LEN(str) && php_check_open_basedir(ZSTR_VAL(str))) {
				zend_string_release(str);
				return FAILURE;
			}

			ret = php_curl_option_str(ch, option, ZSTR_VAL(str), ZSTR_LEN(str), 0);
			zend_string_release(str);
			return ret;
		}

		case CURLINFO_HEADER_OUT:
			lval = zval_get_long(zvalue);
			if (lval == 1) {
				curl_easy_setopt(ch->cp, CURLOPT_DEBUGFUNCTION, curl_debug);
				curl_easy_setopt(ch->cp, CURLOPT_DEBUGDATA, (void *)ch);
				curl_easy_setopt(ch->cp, CURLOPT_VERBOSE, 1);
			} else {
				curl_easy_setopt(ch->cp, CURLOPT_DEBUGFUNCTION, NULL);
				curl_easy_setopt(ch->cp, CURLOPT_DEBUGDATA, NULL);
				curl_easy_setopt(ch->cp, CURLOPT_VERBOSE, 0);
			}
			break;

		case CURLOPT_SHARE:
			{
				php_curlsh *sh;
				if ((sh = (php_curlsh *)zend_fetch_resource_ex(zvalue, le_curl_share_handle_name, le_curl_share_handle))) {
					curl_easy_setopt(ch->cp, CURLOPT_SHARE, sh->share);
				}
			}
			break;

#if LIBCURL_VERSION_NUM >= 0x071500  
		case CURLOPT_FNMATCH_FUNCTION:
			curl_easy_setopt(ch->cp, CURLOPT_FNMATCH_FUNCTION, curl_fnmatch);
			curl_easy_setopt(ch->cp, CURLOPT_FNMATCH_DATA, ch);
			if (ch->handlers->fnmatch == NULL) {
				ch->handlers->fnmatch = ecalloc(1, sizeof(php_curl_fnmatch));
			} else if (!Z_ISUNDEF(ch->handlers->fnmatch->func_name)) {
				zval_ptr_dtor(&ch->handlers->fnmatch->func_name);
				ch->handlers->fnmatch->fci_cache = empty_fcall_info_cache;
			}
			ZVAL_COPY(&ch->handlers->fnmatch->func_name, zvalue);
			ch->handlers->fnmatch->method = PHP_CURL_USER;
			break;
#endif

	}

	SAVE_CURL_ERROR(ch, error);
	if (error != CURLE_OK) {
		return FAILURE;
	} else {
		return SUCCESS;
	}
}
 