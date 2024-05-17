lzh_decode_huffman(struct huffman *hf, unsigned rbits)
{
	int c;
	 
	c = hf->tbl[rbits >> hf->shift_bits];
	if (c < hf->len_avail || hf->len_avail == 0)
		return (c);
	 
	return (lzh_decode_huffman_tree(hf, rbits, c));
}
