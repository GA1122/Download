PHPAPI char *php_escape_shell_cmd(char *str)
{
	register int x, y, l = strlen(str);
	char *cmd;
	char *p = NULL;
	size_t estimate = (2 * l) + 1;

	TSRMLS_FETCH();

	cmd = safe_emalloc(2, l, 1);

	for (x = 0, y = 0; x < l; x++) {
		int mb_len = php_mblen(str + x, (l - x));

		 
		if (mb_len < 0) {
			continue;
		} else if (mb_len > 1) {
			memcpy(cmd + y, str + x, mb_len);
			y += mb_len;
			x += mb_len - 1;
			continue;
		}

		switch (str[x]) {
#ifndef PHP_WIN32
			case '"':
			case '\'':
				if (!p && (p = memchr(str + x + 1, str[x], l - x - 1))) {
					 
				} else if (p && *p == str[x]) {
					p = NULL;
				} else {
					cmd[y++] = '\\';
				}
				cmd[y++] = str[x];
				break;
#else
			 
			case '%':
			case '"':
			case '\'':
#endif
			case '#':  
			case '&':
			case ';':
			case '`':
			case '|':
			case '*':
			case '?':
			case '~':
			case '<':
			case '>':
			case '^':
			case '(':
			case ')':
			case '[':
			case ']':
			case '{':
			case '}':
			case '$':
			case '\\':
			case '\x0A':  
			case '\xFF':
#ifdef PHP_WIN32
				cmd[y++] = '^';
#else
				cmd[y++] = '\\';
#endif
				 
			default:
				cmd[y++] = str[x];

		}
	}
	cmd[y] = '\0';

	if ((estimate - y) > 4096) {
		 
		cmd = erealloc(cmd, y + 1);
	}

	return cmd;
}
