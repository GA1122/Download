static int asn1_encode(sc_context_t *ctx, const struct sc_asn1_entry *asn1,
		      u8 **ptr, size_t *size, int depth)
{
	int r, idx = 0;
	u8 *obj = NULL, *buf = NULL, *tmp;
	size_t total = 0, objsize;

	for (idx = 0; asn1[idx].name != NULL; idx++) {
		r = asn1_encode_entry(ctx, &asn1[idx], &obj, &objsize, depth);
		if (r) {
			if (obj)
				free(obj);
			if (buf)
				free(buf);
			return r;
		}
		 
		if (!objsize)
			continue;
		tmp = (u8 *) realloc(buf, total + objsize);
		if (!tmp) {
			if (obj)
				free(obj);
			if (buf)
				free(buf);
			return SC_ERROR_OUT_OF_MEMORY;
		}
		buf = tmp;
		memcpy(buf + total, obj, objsize);
		free(obj);
		obj = NULL;
		total += objsize;
	}
	*ptr = buf;
	*size = total;
	return 0;
}