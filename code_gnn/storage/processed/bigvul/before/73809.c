static int process_request(
	 
	http_message_t *req,
	 
	enum resp_type *rtype,
	 
	membuffer *headers,
	 
	membuffer *filename,
	 
	struct xml_alias_t *alias,
	 
	struct SendInstruction *RespInstr)
{
	int code;
	int err_code;

	char *request_doc;
	UpnpFileInfo *finfo;
	time_t aux_LastModified;
	int using_alias;
	int using_virtual_dir;
	uri_type *url;
	const char *temp_str;
	int resp_major;
	int resp_minor;
	int alias_grabbed;
	size_t dummy;
	const char *extra_headers = NULL;

	print_http_headers(req);
	url = &req->uri;
	assert(req->method == HTTPMETHOD_GET ||
	       req->method == HTTPMETHOD_HEAD ||
	       req->method == HTTPMETHOD_POST ||
	       req->method == HTTPMETHOD_SIMPLEGET);
	 
	memset(&finfo, 0, sizeof(finfo));
	request_doc = NULL;
	finfo = UpnpFileInfo_new();
	alias_grabbed = FALSE;
	err_code = HTTP_INTERNAL_SERVER_ERROR;	 
	using_virtual_dir = FALSE;
	using_alias = FALSE;

	http_CalcResponseVersion(req->major_version, req->minor_version,
				 &resp_major, &resp_minor);
	 
	 
	 
	request_doc = malloc(url->pathquery.size + 1);
	if (request_doc == NULL) {
		goto error_handler;	 
	}
	memcpy(request_doc, url->pathquery.buff, url->pathquery.size);
	request_doc[url->pathquery.size] = '\0';
	dummy = url->pathquery.size;
	remove_escaped_chars(request_doc, &dummy);
	code = remove_dots(request_doc, url->pathquery.size);
	if (code != 0) {
		err_code = HTTP_FORBIDDEN;
		goto error_handler;
	}
	if (*request_doc != '/') {
		 
		err_code = HTTP_BAD_REQUEST;
		goto error_handler;
	}
	if (isFileInVirtualDir(request_doc)) {
		using_virtual_dir = TRUE;
		RespInstr->IsVirtualFile = 1;
		if (membuffer_assign_str(filename, request_doc) != 0) {
			goto error_handler;
		}
	} else {
		 
		if (is_valid_alias(&gAliasDoc)) {
			alias_grab(alias);
			alias_grabbed = TRUE;
			using_alias = get_alias(request_doc, alias, finfo);
			if (using_alias == TRUE) {
				UpnpFileInfo_set_ContentType(finfo,
					"text/xml; charset=\"utf-8\"");
				if (UpnpFileInfo_get_ContentType(finfo) == NULL) {
					goto error_handler;
				}
			}
		}
	}
	if (using_virtual_dir) {
		if (req->method != HTTPMETHOD_POST) {
			 
			if (virtualDirCallback.
			    get_info(filename->buf, finfo) != 0) {
				err_code = HTTP_NOT_FOUND;
				goto error_handler;
			}
			 
			if (UpnpFileInfo_get_IsDirectory(finfo)) {
				if (filename->buf[filename->length - 1] == '/') {
					temp_str = "index.html";
				} else {
					temp_str = "/index.html";
				}
				if (membuffer_append_str(filename, temp_str) !=
				    0) {
					goto error_handler;
				}
				 
				if (virtualDirCallback.get_info(filename->buf, finfo) != UPNP_E_SUCCESS ||
				    UpnpFileInfo_get_IsDirectory(finfo)) {
					err_code = HTTP_NOT_FOUND;
					goto error_handler;
				}
			}
			 
			if (!UpnpFileInfo_get_IsReadable(finfo)) {
				err_code = HTTP_FORBIDDEN;
				goto error_handler;
			}
			 
			 
			 
			 
			 
		}
	} else if (!using_alias) {
		if (gDocumentRootDir.length == 0) {
			goto error_handler;
		}
		 
		 
		 

		 
		if (membuffer_assign_str(filename, gDocumentRootDir.buf) != 0 ||
		    membuffer_append_str(filename, request_doc) != 0) {
			goto error_handler;	 
		}
		 
		while (filename->length > 0 &&
		       filename->buf[filename->length - 1] == '/') {
			membuffer_delete(filename, filename->length - 1, 1);
		}
		if (req->method != HTTPMETHOD_POST) {
			 
			if (get_file_info(filename->buf, finfo) != 0) {
				err_code = HTTP_NOT_FOUND;
				goto error_handler;
			}
			 
			if (UpnpFileInfo_get_IsDirectory(finfo)) {
				if (filename->buf[filename->length - 1] == '/') {
					temp_str = "index.html";
				} else {
					temp_str = "/index.html";
				}
				if (membuffer_append_str(filename, temp_str) !=
				    0) {
					goto error_handler;
				}
				 
				if (get_file_info(filename->buf, finfo) != 0 ||
				    UpnpFileInfo_get_IsDirectory(finfo)) {
					err_code = HTTP_NOT_FOUND;
					goto error_handler;
				}
			}
			 
			if (!UpnpFileInfo_get_IsReadable(finfo)) {
				err_code = HTTP_FORBIDDEN;
				goto error_handler;
			}
		}
		 
		 
		 
		 
		 
	}
	RespInstr->ReadSendSize = UpnpFileInfo_get_FileLength(finfo);
	 
	code = CheckOtherHTTPHeaders(req, RespInstr,
		UpnpFileInfo_get_FileLength(finfo));
	if (code != HTTP_OK) {
		err_code = code;
		goto error_handler;
	}
	if (req->method == HTTPMETHOD_POST) {
		*rtype = RESP_POST;
		err_code = HTTP_OK;
		goto error_handler;
	}
	extra_headers = UpnpFileInfo_get_ExtraHeaders(finfo);
	if (!extra_headers) {
		extra_headers = "";
	}

	 
	if (using_virtual_dir && UpnpFileInfo_get_FileLength(finfo) == UPNP_USING_CHUNKED) {
		 
		if (resp_major == 1 && resp_minor == 1) {
			RespInstr->IsChunkActive = 1;
		} else {
			 
			err_code = HTTP_NOT_ACCEPTABLE;
			goto error_handler;
		}
	}

	aux_LastModified = UpnpFileInfo_get_LastModified(finfo);
	if (RespInstr->IsRangeActive && RespInstr->IsChunkActive) {
		 
		 
		if (http_MakeMessage(headers, resp_major, resp_minor,
		    "R" "T" "GKLD" "s" "tcS" "Xc" "sCc",
		    HTTP_PARTIAL_CONTENT,	 
		    UpnpFileInfo_get_ContentType(finfo),  
		    RespInstr,	 
		    RespInstr,	 
		    "LAST-MODIFIED: ",
		    &aux_LastModified,
		    X_USER_AGENT, extra_headers) != 0) {
			goto error_handler;
		}
	} else if (RespInstr->IsRangeActive && !RespInstr->IsChunkActive) {
		 
		if (http_MakeMessage(headers, resp_major, resp_minor,
		    "R" "N" "T" "GLD" "s" "tcS" "Xc" "sCc",
		    HTTP_PARTIAL_CONTENT,	 
		    RespInstr->ReadSendSize,	 
		    UpnpFileInfo_get_ContentType(finfo),  
		    RespInstr,	 
		    RespInstr,	 
		    "LAST-MODIFIED: ",
		    &aux_LastModified,
		    X_USER_AGENT, extra_headers) != 0) {
			goto error_handler;
		}
	} else if (!RespInstr->IsRangeActive && RespInstr->IsChunkActive) {
		 
		if (http_MakeMessage(headers, resp_major, resp_minor,
		    "RK" "TLD" "s" "tcS" "Xc" "sCc",
		    HTTP_OK,	 
		    UpnpFileInfo_get_ContentType(finfo),  
		    RespInstr,	 
		    "LAST-MODIFIED: ",
		    &aux_LastModified,
		    X_USER_AGENT, extra_headers) != 0) {
			goto error_handler;
		}
	} else {
		 
		if (RespInstr->ReadSendSize >= 0) {
			if (http_MakeMessage(headers, resp_major, resp_minor,
			    "R" "N" "TLD" "s" "tcS" "Xc" "sCc",
			    HTTP_OK,	 
			    RespInstr->ReadSendSize,	 
			    UpnpFileInfo_get_ContentType(finfo),  
			    RespInstr,	 
			    "LAST-MODIFIED: ",
			    &aux_LastModified,
			    X_USER_AGENT,
			    extra_headers) != 0) {
				goto error_handler;
			}
		} else {
			if (http_MakeMessage(headers, resp_major, resp_minor,
			    "R" "TLD" "s" "tcS" "Xc" "sCc",
			    HTTP_OK,	 
			    UpnpFileInfo_get_ContentType(finfo),  
			    RespInstr,	 
			    "LAST-MODIFIED: ",
			    &aux_LastModified,
			    X_USER_AGENT,
			    extra_headers) != 0) {
				goto error_handler;
			}
		}
	}
	if (req->method == HTTPMETHOD_HEAD) {
		*rtype = RESP_HEADERS;
	} else if (using_alias) {
		 
		*rtype = RESP_XMLDOC;
	} else if (using_virtual_dir) {
		*rtype = RESP_WEBDOC;
	} else {
		 
		*rtype = RESP_FILEDOC;
	}
	 
	 
	if (req->method == HTTPMETHOD_SIMPLEGET) {
		membuffer_destroy(headers);
	}
	err_code = HTTP_OK;

 error_handler:
	free(request_doc);
	UpnpFileInfo_delete(finfo);
	if (err_code != HTTP_OK && alias_grabbed) {
		alias_release(alias);
	}

	return err_code;
}
