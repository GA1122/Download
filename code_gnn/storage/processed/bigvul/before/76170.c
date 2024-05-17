keyword_alloc_sub(vector_t *keywords_vec, const char *string, void (*handler) (vector_t *))
{
	int i = 0;
	keyword_t *keyword;

	 
	keyword = vector_slot(keywords_vec, vector_size(keywords_vec) - 1);

	 
	if (!keyword->active)
		return;

	 
	for (i = 0; i < sublevel; i++)
		keyword = vector_slot(keyword->sub, vector_size(keyword->sub) - 1);

	 
	if (!keyword->sub)
		keyword->sub = vector_alloc();

	 
	keyword_alloc(keyword->sub, string, handler, true);
}
