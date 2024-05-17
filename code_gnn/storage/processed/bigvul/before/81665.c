extention_matches_script(
    struct mg_connection *conn,  
    const char *filename         
    )
{
#if !defined(NO_CGI)
	if (match_prefix(conn->dom_ctx->config[CGI_EXTENSIONS],
	                 strlen(conn->dom_ctx->config[CGI_EXTENSIONS]),
	                 filename) > 0) {
		return 1;
	}
#endif
#if defined(USE_LUA)
	if (match_prefix(conn->dom_ctx->config[LUA_SCRIPT_EXTENSIONS],
	                 strlen(conn->dom_ctx->config[LUA_SCRIPT_EXTENSIONS]),
	                 filename) > 0) {
		return 1;
	}
#endif
#if defined(USE_DUKTAPE)
	if (match_prefix(conn->dom_ctx->config[DUKTAPE_SCRIPT_EXTENSIONS],
	                 strlen(conn->dom_ctx->config[DUKTAPE_SCRIPT_EXTENSIONS]),
	                 filename) > 0) {
		return 1;
	}
#endif
	 
	(void)filename;
	(void)conn;

	return 0;
}