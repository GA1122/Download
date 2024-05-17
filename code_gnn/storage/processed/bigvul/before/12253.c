static char *get_http_header_value(char *headers, char *type)
{
	char *pos, *tmp = NULL;
	int typelen, headerslen;

	typelen = strlen(type);
	headerslen = strlen(headers);

	 
	pos = headers;
	do {
		 
		if (strncasecmp(pos, type, typelen) == 0) {
			char *eol;

			 
			tmp = pos + typelen;
			eol = strchr(tmp, '\n');
			if (eol == NULL) {
				eol = headers + headerslen;
			} else if (eol > tmp && *(eol-1) == '\r') {
				eol--;
			}
			return estrndup(tmp, eol - tmp);
		}

		 
		pos = strchr(pos, '\n');
		if (pos) {
			pos++;
		}

	} while (pos);

	return NULL;
}
