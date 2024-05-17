rd_contents_init(struct archive_read *a, enum enctype encoding,
    int a_sum_alg, int e_sum_alg)
{
	int r;

	 
	if ((r = decompression_init(a, encoding)) != ARCHIVE_OK)
		return (r);
	 
	checksum_init(a, a_sum_alg, e_sum_alg);
	return (ARCHIVE_OK);
}
