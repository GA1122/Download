static const tls12_hash_info *tls12_get_hash_info(unsigned char hash_alg)
	{
	if (hash_alg == 0)
		return NULL;
	if (hash_alg > sizeof(tls12_md_info)/sizeof(tls12_md_info[0]))
		return NULL;
	return tls12_md_info + hash_alg - 1;
	}
