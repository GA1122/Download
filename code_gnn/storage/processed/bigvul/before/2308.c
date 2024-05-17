_PUBLIC_ bool strcsequal(const char *s1,const char *s2)
{
	if (s1 == s2)
		return true;
	if (!s1 || !s2)
		return false;

	return strcmp(s1,s2) == 0;
}
