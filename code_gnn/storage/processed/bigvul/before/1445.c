void exp2msg(const struct nf_expect *exp, struct nethdr *n)
{
	const struct nf_conntrack *ct = nfexp_get_attr(exp, ATTR_EXP_MASTER);
	uint8_t l4proto = nfct_get_attr_u8(ct, ATTR_L4PROTO);

	 
	if (nfct_attr_grp_is_set(ct, ATTR_GRP_ORIG_IPV4)) {
		ct_build_group(ct, ATTR_GRP_ORIG_IPV4, n, NTA_EXP_MASTER_IPV4,
			      sizeof(struct nfct_attr_grp_ipv4));
	} else if (nfct_attr_grp_is_set(ct, ATTR_GRP_ORIG_IPV6)) {
		ct_build_group(ct, ATTR_GRP_ORIG_IPV6, n, NTA_EXP_MASTER_IPV6,
			      sizeof(struct nfct_attr_grp_ipv6));
	}
	ct_build_u8(ct, ATTR_L4PROTO, n, NTA_EXP_MASTER_L4PROTO);

	if (exp_l4proto_fcn[l4proto].build)
		exp_l4proto_fcn[l4proto].build(ct, n, NTA_EXP_MASTER_PORT);

	 
	ct = nfexp_get_attr(exp, ATTR_EXP_EXPECTED);

	if (nfct_attr_grp_is_set(ct, ATTR_GRP_ORIG_IPV4)) {
		ct_build_group(ct, ATTR_GRP_ORIG_IPV4, n, NTA_EXP_EXPECT_IPV4,
			      sizeof(struct nfct_attr_grp_ipv4));
	} else if (nfct_attr_grp_is_set(ct, ATTR_GRP_ORIG_IPV6)) {
		ct_build_group(ct, ATTR_GRP_ORIG_IPV6, n, NTA_EXP_EXPECT_IPV6,
			      sizeof(struct nfct_attr_grp_ipv6));
	}
	ct_build_u8(ct, ATTR_L4PROTO, n, NTA_EXP_EXPECT_L4PROTO);

	if (exp_l4proto_fcn[l4proto].build)
		exp_l4proto_fcn[l4proto].build(ct, n, NTA_EXP_EXPECT_PORT);

	 
	ct = nfexp_get_attr(exp, ATTR_EXP_MASK);

	if (nfct_attr_grp_is_set(ct, ATTR_GRP_ORIG_IPV4)) {
		ct_build_group(ct, ATTR_GRP_ORIG_IPV4, n, NTA_EXP_MASK_IPV4,
			      sizeof(struct nfct_attr_grp_ipv4));
	} else if (nfct_attr_grp_is_set(ct, ATTR_GRP_ORIG_IPV6)) {
		ct_build_group(ct, ATTR_GRP_ORIG_IPV6, n, NTA_EXP_MASK_IPV6,
			      sizeof(struct nfct_attr_grp_ipv6));
	}
	ct_build_u8(ct, ATTR_L4PROTO, n, NTA_EXP_MASK_L4PROTO);

	if (exp_l4proto_fcn[l4proto].build)
		exp_l4proto_fcn[l4proto].build(ct, n, NTA_EXP_MASK_PORT);

	if (!CONFIG(commit_timeout) && nfexp_attr_is_set(exp, ATTR_EXP_TIMEOUT))
		exp_build_u32(exp, ATTR_EXP_TIMEOUT, n, NTA_EXP_TIMEOUT);

	exp_build_u32(exp, ATTR_EXP_FLAGS, n, NTA_EXP_FLAGS);
	if (nfexp_attr_is_set(exp, ATTR_EXP_CLASS))
		exp_build_u32(exp, ATTR_EXP_CLASS, n, NTA_EXP_CLASS);

	 
	ct = nfexp_get_attr(exp, ATTR_EXP_NAT_TUPLE);
	if (ct != NULL) {
		if (nfct_attr_grp_is_set(ct, ATTR_GRP_ORIG_IPV4)) {
			ct_build_group(ct, ATTR_GRP_ORIG_IPV4, n,
					NTA_EXP_NAT_IPV4,
					sizeof(struct nfct_attr_grp_ipv4));
		}
		ct_build_u8(ct, ATTR_L4PROTO, n, NTA_EXP_NAT_L4PROTO);
		if (exp_l4proto_fcn[l4proto].build)
			exp_l4proto_fcn[l4proto].build(ct, n, NTA_EXP_NAT_PORT);

		exp_build_u32(exp, ATTR_EXP_NAT_DIR, n, NTA_EXP_NAT_DIR);
	}
	if (nfexp_attr_is_set(exp, ATTR_EXP_HELPER_NAME))
		exp_build_str(exp, ATTR_EXP_HELPER_NAME, n, NTA_EXP_HELPER_NAME);
	if (nfexp_attr_is_set(exp, ATTR_EXP_FN))
		exp_build_str(exp, ATTR_EXP_FN, n, NTA_EXP_FN);
}
