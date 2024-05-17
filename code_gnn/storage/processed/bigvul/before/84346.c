UNCURL_EXPORT void uncurl_set_header_str(struct uncurl_conn *ucc, char *name, char *value)
{
	ucc->hout = http_set_header(ucc->hout, name, HTTP_STRING, value);
}
