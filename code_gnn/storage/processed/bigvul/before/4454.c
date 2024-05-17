static inline int in_character_class(char ch, const char *delim)  
{
	while (*delim) {
		if (*delim == ch) {
			return 1;
		}
		++delim;
	}
	return 0;
}
 