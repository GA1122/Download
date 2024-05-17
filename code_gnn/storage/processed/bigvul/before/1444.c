ct_build_u8(const struct nf_conntrack *ct, int a, struct nethdr *n, int b)
{
	void *ptr = put_header(n, b, sizeof(uint8_t));
	memcpy(ptr, nfct_get_attr(ct, a), sizeof(uint8_t));
}
