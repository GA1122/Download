input_userauth_banner(int type, u_int32_t seq, void *ctxt)
{
	char *msg, *lang;
	u_int len;

	debug3("%s", __func__);
	msg = packet_get_string(&len);
	lang = packet_get_string(NULL);
	if (len > 0 && options.log_level >= SYSLOG_LEVEL_INFO)
		fmprintf(stderr, "%s", msg);
	free(msg);
	free(lang);
	return 0;
}
