mtree_bid(struct archive_read *a, int best_bid)
{
	const char *signature = "#mtree";
	const char *p;

	(void)best_bid;  

	 
	p = __archive_read_ahead(a, strlen(signature), NULL);
	if (p == NULL)
		return (-1);

	if (memcmp(p, signature, strlen(signature)) == 0)
		return (8 * (int)strlen(signature));

	 
	return (detect_form(a, NULL));
}
