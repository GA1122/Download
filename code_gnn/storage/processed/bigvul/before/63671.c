char **strsplit_len(const char *str, int len, gboolean onspace)
{
	char **ret = g_new(char *, 1);
	int n;
	int offset;

	for (n = 0; *str != '\0'; n++, str += offset) {
		offset = MIN(len, strlen(str));
		if (onspace && strlen(str) > len) {
			 
			int i;
			for (i = len - 1; i > 0; i--) {
				if (str[i] == ' ') {
					offset = i;
					break;
				}
			}
		}
		ret[n] = g_strndup(str, offset);
		ret = g_renew(char *, ret, n + 2);
	}
	ret[n] = NULL;

	return ret;
}