static bool BB_isalnum_or_underscore(CHAR_T c) {
	return ((unsigned)c < 256 && isalnum(c)) || c == '_';
}
