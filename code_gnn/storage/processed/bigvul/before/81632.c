check_authorization(struct mg_connection *conn, const char *path)
{
	char fname[PATH_MAX];
	struct vec uri_vec, filename_vec;
	const char *list;
	struct mg_file file = STRUCT_FILE_INITIALIZER;
	int authorized = 1, truncated;

	if (!conn || !conn->dom_ctx) {
		return 0;
	}

	list = conn->dom_ctx->config[PROTECT_URI];
	while ((list = next_option(list, &uri_vec, &filename_vec)) != NULL) {
		if (!memcmp(conn->request_info.local_uri, uri_vec.ptr, uri_vec.len)) {
			mg_snprintf(conn,
			            &truncated,
			            fname,
			            sizeof(fname),
			            "%.*s",
			            (int)filename_vec.len,
			            filename_vec.ptr);

			if (truncated
			    || !mg_fopen(conn, fname, MG_FOPEN_MODE_READ, &file)) {
				mg_cry_internal(conn,
				                "%s: cannot open %s: %s",
				                __func__,
				                fname,
				                strerror(errno));
			}
			break;
		}
	}

	if (!is_file_opened(&file.access)) {
		open_auth_file(conn, path, &file);
	}

	if (is_file_opened(&file.access)) {
		authorized = authorize(conn, &file, NULL);
		(void)mg_fclose(&file.access);  
	}

	return authorized;
}