static void init_arraycache(struct array_cache *ac, int limit, int batch)
{
	 
	kmemleak_no_scan(ac);
	if (ac) {
		ac->avail = 0;
		ac->limit = limit;
		ac->batchcount = batch;
		ac->touched = 0;
	}
}
