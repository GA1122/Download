static int check_pad_bytes(struct kmem_cache *s, struct page *page, u8 *p)
{
	unsigned long off = s->inuse;	 

	if (s->offset)
		 
		off += sizeof(void *);

	if (s->flags & SLAB_STORE_USER)
		 
		off += 2 * sizeof(struct track);

	if (s->size == off)
		return 1;

	return check_bytes_and_report(s, page, p, "Object padding",
				p + off, POISON_INUSE, s->size - off);
}