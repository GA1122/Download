delete_file(struct mg_connection *conn, const char *path)
{
	struct de de;
	memset(&de.file, 0, sizeof(de.file));
	if (!mg_stat(conn, path, &de.file)) {
		 
		mg_send_http_error(conn,
		                   404,
		                   "Error: Cannot delete file\nFile %s not found",
		                   path);
		return;
	}

#if 0  
        if (de.access.membuf != NULL) {
                 
                mg_send_http_error(
                    conn,
                    405,
                    "Error: Delete not possible\nDeleting %s is not supported",
                    path);
                return;
        }
#endif

	if (de.file.is_directory) {
		if (remove_directory(conn, path)) {
			 
			mg_send_http_error(conn, 204, "%s", "");
		} else {
			 
			mg_send_http_error(conn, 500, "Error: Could not delete %s", path);
		}
		return;
	}

	 
	if (access(path, W_OK) != 0) {
		 
		mg_send_http_error(
		    conn,
		    403,
		    "Error: Delete not possible\nDeleting %s is not allowed",
		    path);
		return;
	}

	 
	if (mg_remove(conn, path) == 0) {
		 
		mg_send_http_error(conn, 204, "%s", "");
	} else {
		 
		mg_send_http_error(conn,
		                   423,
		                   "Error: Cannot delete file\nremove(%s): %s",
		                   path,
		                   strerror(ERRNO));
	}
}
