static inline php_uint32 ps_sd_hash(const char *data, int len)
{
	php_uint32 h;
	const char *e = data + len;

	for (h = 2166136261U; data < e; ) {
		h *= 16777619;
		h ^= *data++;
	}

	return h;
}
