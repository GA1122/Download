hash_cached_sock_errs(const struct cached_sock_errs_entry *e)
{
	 
	DWORD h = e->code;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}
