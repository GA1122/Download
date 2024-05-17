ct_build_str(const struct nf_conntrack *ct, int a, struct nethdr *n, int b)
{
	const char *data = nfct_get_attr(ct, a);
	addattr(n, b, data, strlen(data)+1);
}
