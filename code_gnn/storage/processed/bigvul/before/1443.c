ct_build_u16(const struct nf_conntrack *ct, int a, struct nethdr *n, int b)
{
	uint16_t data = nfct_get_attr_u16(ct, a);
	data = htons(data);
	addattr(n, b, &data, sizeof(uint16_t));
}
