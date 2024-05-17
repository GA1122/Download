char *phar_fix_filepath(char *path, int *new_len, int use_cwd)  
{
	char *newpath;
	int newpath_len;
	char *ptr;
	char *tok;
	int ptr_length, path_length = *new_len;

	if (PHAR_G(cwd_len) && use_cwd && path_length > 2 && path[0] == '.' && path[1] == '/') {
		newpath_len = PHAR_G(cwd_len);
		newpath = emalloc(strlen(path) + newpath_len + 1);
		memcpy(newpath, PHAR_G(cwd), newpath_len);
	} else {
		newpath = emalloc(strlen(path) + 2);
		newpath[0] = '/';
		newpath_len = 1;
	}

	ptr = path;

	if (*ptr == '/') {
		++ptr;
	}

	tok = ptr;

	do {
		ptr = memchr(ptr, '/', path_length - (ptr - path));
	} while (ptr && ptr - tok == 0 && *ptr == '/' && ++ptr && ++tok);

	if (!ptr && (path_length - (tok - path))) {
		switch (path_length - (tok - path)) {
			case 1:
				if (*tok == '.') {
					efree(path);
					*new_len = 1;
					efree(newpath);
					return estrndup("/", 1);
				}
				break;
			case 2:
				if (tok[0] == '.' && tok[1] == '.') {
					efree(path);
					*new_len = 1;
					efree(newpath);
					return estrndup("/", 1);
				}
		}
		efree(newpath);
		return path;
	}

	while (ptr) {
		ptr_length = ptr - tok;
last_time:
		if (IS_DIRECTORY_UP(tok, ptr_length)) {
#define PREVIOUS newpath[newpath_len - 1]

			while (newpath_len > 1 && !IS_BACKSLASH(PREVIOUS)) {
				newpath_len--;
			}

			if (newpath[0] != '/') {
				newpath[newpath_len] = '\0';
			} else if (newpath_len > 1) {
				--newpath_len;
			}
		} else if (!IS_DIRECTORY_CURRENT(tok, ptr_length)) {
			if (newpath_len > 1) {
				newpath[newpath_len++] = '/';
				memcpy(newpath + newpath_len, tok, ptr_length+1);
			} else {
				memcpy(newpath + newpath_len, tok, ptr_length+1);
			}

			newpath_len += ptr_length;
		}

		if (ptr == path + path_length) {
			break;
		}

		tok = ++ptr;

		do {
			ptr = memchr(ptr, '/', path_length - (ptr - path));
		} while (ptr && ptr - tok == 0 && *ptr == '/' && ++ptr && ++tok);

		if (!ptr && (path_length - (tok - path))) {
			ptr_length = path_length - (tok - path);
			ptr = path + path_length;
			goto last_time;
		}
	}

	efree(path);
	*new_len = newpath_len;
	newpath[newpath_len] = '\0';
	return erealloc(newpath, newpath_len + 1);
}
 