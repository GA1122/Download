char *_dom_get_valid_file_path(char *source, char *resolved_path, int resolved_path_len  TSRMLS_DC)  
{
	xmlURI *uri;
	xmlChar *escsource;
	char *file_dest;
	int isFileUri = 0;

	uri = xmlCreateURI();
	escsource = xmlURIEscapeStr(source, ":");
	xmlParseURIReference(uri, escsource);
	xmlFree(escsource);

	if (uri->scheme != NULL) {
		 
#ifdef PHP_WIN32
		if (strncasecmp(source, "file://",7) == 0 && ':' == source[8]) {
			isFileUri = 1;
			source += 7;
		} else
#endif
		if (strncasecmp(source, "file:///",8) == 0) {
			isFileUri = 1;
#ifdef PHP_WIN32
			source += 8;
#else
			source += 7;
#endif
		} else if (strncasecmp(source, "file://localhost/",17) == 0) {
			isFileUri = 1;
#ifdef PHP_WIN32
			source += 17;
#else
			source += 16;
#endif
		}
	}

	file_dest = source;

	if ((uri->scheme == NULL || isFileUri)) {
		 
		if (!VCWD_REALPATH(source, resolved_path) && !expand_filepath(source, resolved_path TSRMLS_CC)) {
			xmlFreeURI(uri);
			return NULL;
		}
		file_dest = resolved_path;
	}

	xmlFreeURI(uri);

	return file_dest;
}
 
