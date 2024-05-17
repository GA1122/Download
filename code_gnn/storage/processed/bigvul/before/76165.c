install_keyword_root(const char *string, void (*handler) (vector_t *), bool active)
{
	 
	keyword_alloc(keywords, string, handler, active);
}
