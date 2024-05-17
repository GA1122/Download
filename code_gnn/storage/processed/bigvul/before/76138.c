set_script_params_array(vector_t *strvec, notify_script_t *script, unsigned extra_params)
{
	unsigned num_words = 0;
	size_t len = 0;
	char **word_ptrs;
	char *words;
	vector_t *strvec_qe = NULL;
	unsigned i;

	 
	if (vector_size(strvec) >= 2)
		strvec_qe = alloc_strvec_quoted_escaped(strvec_slot(strvec, 1));

	if (!strvec_qe)
		return;

	num_words = vector_size(strvec_qe);
	for (i = 0; i < num_words; i++)
		len += strlen(strvec_slot(strvec_qe, i)) + 1;

	 
	script->args = word_ptrs = MALLOC((num_words + extra_params + 1) * sizeof(char *) + len);
	words = (char *)word_ptrs + (num_words + extra_params + 1) * sizeof(char *);

	 
	for (i = 0; i < num_words; i++) {
		strcpy(words, strvec_slot(strvec_qe, i));
		*(word_ptrs++) = words;
		words += strlen(words) + 1;
	}
	*word_ptrs = NULL;

	script->num_args = num_words;

	free_strvec(strvec_qe);
}