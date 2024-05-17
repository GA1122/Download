static char *php_pgsql_PQescapeInternal(PGconn *conn, const char *str, size_t len, int escape_literal, int safe)  
{
	char *result, *rp, *s;
	size_t tmp_len;

	if (!conn) {
		return NULL;
	}

	 
	rp = result = (char *)safe_emalloc(len, 2, 5);  

	if (escape_literal) {
		size_t new_len;

		if (safe) {
			char *tmp = (char *)safe_emalloc(len, 2, 1);
			*rp++ = '\'';
			 
			new_len = PQescapeStringConn(conn, tmp, str, len, NULL);
			strncpy(rp, tmp, new_len);
			efree(tmp);
			rp += new_len;
		} else {
			char *encoding;
			 
			encoding = (char *) pg_encoding_to_char(PQclientEncoding(conn));
			if (!strncmp(encoding, "SJIS", sizeof("SJIS")-1) ||
				!strncmp(encoding, "SHIFT_JIS_2004", sizeof("SHIFT_JIS_2004")-1) ||
				!strncmp(encoding, "BIG5", sizeof("BIG5")-1) ||
				!strncmp(encoding, "GB18030", sizeof("GB18030")-1) ||
				!strncmp(encoding, "GBK", sizeof("GBK")-1) ||
				!strncmp(encoding, "JOHAB", sizeof("JOHAB")-1) ||
				!strncmp(encoding, "UHC", sizeof("UHC")-1) ) {
			
				php_error_docref(NULL, E_WARNING, "Unsafe encoding is used. Do not use '%s' encoding or use PostgreSQL 9.0 or later libpq.", encoding);
			}
			 
			tmp_len = strspn(str, "\\");
			if (tmp_len != len) {
				 
				*rp++ = ' ';
				*rp++ = 'E';
			}
			*rp++ = '\'';
			for (s = (char *)str; s - str < len; ++s) {
				if (*s == '\'' || *s == '\\') {
					*rp++ = *s;
					*rp++ = *s;
				} else {
					*rp++ = *s;
				}
			}
		}
		*rp++ = '\'';
	} else {
		 
		*rp++ = '"';
		for (s = (char *)str; s - str < len; ++s) {
			if (*s == '"') {
				*rp++ = '"';
				*rp++ = '"';
			} else {
				*rp++ = *s;
			}
		}
		*rp++ = '"';
	}
	*rp = '\0';

	return result;
}
 
