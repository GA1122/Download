static Indirect *ext4_find_shared(struct inode *inode, int depth,
				  ext4_lblk_t offsets[4], Indirect chain[4],
				  __le32 *top)
{
	Indirect *partial, *p;
	int k, err;

	*top = 0;
	 
	for (k = depth; k > 1 && !offsets[k-1]; k--)
		;
	partial = ext4_get_branch(inode, k, offsets, chain, &err);
	 
	if (!partial)
		partial = chain + k-1;
	 
	if (!partial->key && *partial->p)
		 
		goto no_top;
	for (p = partial; (p > chain) && all_zeroes((__le32 *) p->bh->b_data, p->p); p--)
		;
	 
	if (p == chain + k - 1 && p > chain) {
		p->p--;
	} else {
		*top = *p->p;
		 
#if 0
		*p->p = 0;
#endif
	}
	 

	while (partial > p) {
		brelse(partial->bh);
		partial--;
	}
no_top:
	return partial;
}
