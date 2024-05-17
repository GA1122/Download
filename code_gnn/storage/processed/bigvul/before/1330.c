static uint32_t *nfs_add_fh3(uint32_t *p, struct nfs_fh *fh)
{
	*p++ = hton32(fh->size);

	 
	if (fh->size & 3)
		p[fh->size / 4] = 0;

	memcpy(p, fh->data, fh->size);
	p += DIV_ROUND_UP(fh->size, 4);
	return p;
}
