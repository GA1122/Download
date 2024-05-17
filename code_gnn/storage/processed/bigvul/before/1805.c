static int prefixmatch(const struct in6_addr *s, const struct in6_addr *d)
{
	 
	unsigned i;
	for (i=0; i<128 && !((s->s6_addr[i/8]^d->s6_addr[i/8])&(128>>(i%8))); i++);
	return i;
}
