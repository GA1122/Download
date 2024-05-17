IW_IMPL(char*) iw_get_libjpeg_version_string(char *s, int s_len)
{
	struct jpeg_error_mgr jerr;
	const char *jv;
	char *space_ptr;

	jpeg_std_error(&jerr);
	jv = jerr.jpeg_message_table[JMSG_VERSION];
	iw_snprintf(s,s_len,"%s",jv);

	space_ptr = strchr(s,' ');
	if(space_ptr) *space_ptr = '\0';
	return s;
}
