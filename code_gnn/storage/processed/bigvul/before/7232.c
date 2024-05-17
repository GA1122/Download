static void hash_split(ps_mm *data)
{
	php_uint32 nmax;
	ps_sd **nhash;
	ps_sd **ohash, **ehash;
	ps_sd *ps, *next;

	nmax = ((data->hash_max + 1) << 1) - 1;
	nhash = mm_calloc(data->mm, nmax + 1, sizeof(*data->hash));

	if (!nhash) {
		 
		return;
	}

	ehash = data->hash + data->hash_max + 1;
	for (ohash = data->hash; ohash < ehash; ohash++) {
		for (ps = *ohash; ps; ps = next) {
			next = ps->next;
			ps->next = nhash[ps->hv & nmax];
			nhash[ps->hv & nmax] = ps;
		}
	}
	mm_free(data->mm, data->hash);

	data->hash = nhash;
	data->hash_max = nmax;
}
