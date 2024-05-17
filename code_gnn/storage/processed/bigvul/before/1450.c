exp_build_str(const struct nf_expect *exp, int a, struct nethdr *n, int b)
{
	const char *data = nfexp_get_attr(exp, a);
	addattr(n, b, data, strlen(data)+1);
}
