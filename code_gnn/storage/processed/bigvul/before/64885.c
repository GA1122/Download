static size_t rle4_get_best_unc_split(size_t n)
{
	if(n<=255) return n;

	if(n==257 || n==510 || n==765) return 255;

	return 252;
}
