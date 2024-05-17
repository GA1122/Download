getstr(struct magic_set *ms, struct magic *m, const char *s, int warn)
{
	const char *origs = s;
	char	*p = m->value.s;
	size_t  plen = sizeof(m->value.s);
	char 	*origp = p;
	char	*pmax = p + plen - 1;
	int	c;
	int	val;

	while ((c = *s++) != '\0') {
		if (isspace((unsigned char) c))
			break;
		if (p >= pmax) {
			file_error(ms, 0, "string too long: `%s'", origs);
			return NULL;
		}
		if (c == '\\') {
			switch(c = *s++) {

			case '\0':
				if (warn)
					file_magwarn(ms, "incomplete escape");
				goto out;

			case '\t':
				if (warn) {
					file_magwarn(ms,
					    "escaped tab found, use \\t instead");
					warn = 0;	 
				}
				 
			default:
				if (warn) {
					if (isprint((unsigned char)c)) {
						 
						if (strchr("<>&^=!", c) == NULL
						    && (m->type != FILE_REGEX ||
						    strchr("[]().*?^$|{}", c)
						    == NULL)) {
							file_magwarn(ms, "no "
							    "need to escape "
							    "`%c'", c);
						}
					} else {
						file_magwarn(ms,
						    "unknown escape sequence: "
						    "\\%03o", c);
					}
				}
				 
			 
			case ' ':
#if 0
			 
			case '\'':
			case '"':
			case '?':
#endif
			 
			case '>':
			case '<':
			case '&':
			case '^':
			case '=':
			case '!':
			 
			case '\\':
				*p++ = (char) c;
				break;

			case 'a':
				*p++ = '\a';
				break;

			case 'b':
				*p++ = '\b';
				break;

			case 'f':
				*p++ = '\f';
				break;

			case 'n':
				*p++ = '\n';
				break;

			case 'r':
				*p++ = '\r';
				break;

			case 't':
				*p++ = '\t';
				break;

			case 'v':
				*p++ = '\v';
				break;

			 
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				val = c - '0';
				c = *s++;   
				if (c >= '0' && c <= '7') {
					val = (val << 3) | (c - '0');
					c = *s++;   
					if (c >= '0' && c <= '7')
						val = (val << 3) | (c-'0');
					else
						--s;
				}
				else
					--s;
				*p++ = (char)val;
				break;

			 
			case 'x':
				val = 'x';	 
				c = hextoint(*s++);	 
				if (c >= 0) {
					val = c;
					c = hextoint(*s++);
					if (c >= 0)
						val = (val << 4) + c;
					else
						--s;
				} else
					--s;
				*p++ = (char)val;
				break;
			}
		} else
			*p++ = (char)c;
	}
out:
	*p = '\0';
	m->vallen = CAST(unsigned char, (p - origp));
	if (m->type == FILE_PSTRING)
		m->vallen += (unsigned char)file_pstring_length_size(m);
	return s;
}