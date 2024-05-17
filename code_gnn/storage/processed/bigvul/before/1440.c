ct_build_group(const struct nf_conntrack *ct, int a, struct nethdr *n, 
	      int b, int size)
{
	void *ptr = put_header(n, b, size);
	nfct_get_attr_grp(ct, a, ptr);
}
