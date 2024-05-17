static char *_xml_decode_tag(xml_parser *parser, const char *tag)
{
	char *newstr;
	int out_len;

	newstr = xml_utf8_decode(tag, strlen(tag), &out_len, parser->target_encoding);

	if (parser->case_folding) {
		php_strtoupper(newstr, out_len);
	}

	return newstr;
}
