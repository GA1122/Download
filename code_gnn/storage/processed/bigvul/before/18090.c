static void huft_free(huft_t *p)
{
	huft_t *q;

	 
	while (p) {
		q = (--p)->v.t;
		free(p);
		p = q;
	}
}
