static int cipso_v4_map_cat_rng_hton(const struct cipso_v4_doi *doi_def,
				     const struct netlbl_lsm_secattr *secattr,
				     unsigned char *net_cat,
				     u32 net_cat_len)
{
	int iter = -1;
	u16 array[CIPSO_V4_TAG_RNG_CAT_MAX * 2];
	u32 array_cnt = 0;
	u32 cat_size = 0;

	 
	if (net_cat_len >
	    (CIPSO_V4_OPT_LEN_MAX - CIPSO_V4_HDR_LEN - CIPSO_V4_TAG_RNG_BLEN))
		return -ENOSPC;

	for (;;) {
		iter = netlbl_secattr_catmap_walk(secattr->attr.mls.cat,
						  iter + 1);
		if (iter < 0)
			break;
		cat_size += (iter == 0 ? 0 : sizeof(u16));
		if (cat_size > net_cat_len)
			return -ENOSPC;
		array[array_cnt++] = iter;

		iter = netlbl_secattr_catmap_walk_rng(secattr->attr.mls.cat,
						      iter);
		if (iter < 0)
			return -EFAULT;
		cat_size += sizeof(u16);
		if (cat_size > net_cat_len)
			return -ENOSPC;
		array[array_cnt++] = iter;
	}

	for (iter = 0; array_cnt > 0;) {
		*((__be16 *)&net_cat[iter]) = htons(array[--array_cnt]);
		iter += 2;
		array_cnt--;
		if (array[array_cnt] != 0) {
			*((__be16 *)&net_cat[iter]) = htons(array[array_cnt]);
			iter += 2;
		}
	}

	return cat_size;
}