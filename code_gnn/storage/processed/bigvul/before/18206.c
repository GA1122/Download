static char *server_create_tag(SERVER_CONNECT_REC *conn)
{
	GString *str;
	char *tag;
	int num;

	g_return_val_if_fail(IS_SERVER_CONNECT(conn), NULL);

	tag = conn->chatnet != NULL && *conn->chatnet != '\0' ?
		g_strdup(conn->chatnet) :
		server_create_address_tag(conn->address);

	if (conn->tag != NULL && server_find_tag(conn->tag) == NULL &&
            server_find_lookup_tag(conn->tag) == NULL &&
	    strncmp(conn->tag, tag, strlen(tag)) == 0) {
		 
		g_free(tag);
		return g_strdup(conn->tag);
	}


	 
	str = g_string_new(tag);

	num = 2;
	while (server_find_tag(str->str) != NULL ||
	       server_find_lookup_tag(str->str) != NULL) {
		g_string_printf(str, "%s%d", tag, num);
		num++;
	}
	g_free(tag);

	tag = str->str;
	g_string_free(str, FALSE);
	return tag;
}
