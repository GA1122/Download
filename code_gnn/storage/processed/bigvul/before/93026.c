rdp_out_unistr_mandatory_null(STREAM s, char *string, int len)
{
	 
	if (string && len > 0)
		rdp_out_unistr(s, string, len);
	else
		out_uint16_le(s, 0);
}
