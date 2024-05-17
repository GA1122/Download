input_gssapi_error(int type, u_int32_t plen, void *ctxt)
{
	char *msg;
	char *lang;

	 (void)packet_get_int();
	 (void)packet_get_int();
	msg=packet_get_string(NULL);
	lang=packet_get_string(NULL);

	packet_check_eom();

	debug("Server GSSAPI Error:\n%s", msg);
	free(msg);
	free(lang);
	return 0;
}
