exp_build_u32(const struct nf_expect *exp, int a, struct nethdr *n, int b)
{
	uint32_t data = nfexp_get_attr_u32(exp, a);
	data = htonl(data);
	addattr(n, b, &data, sizeof(uint32_t));
}
