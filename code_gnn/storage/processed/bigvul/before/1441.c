ct_build_natseqadj(const struct nf_conntrack *ct, struct nethdr *n)
{
	struct nta_attr_natseqadj data = {
		.orig_seq_correction_pos =
		htonl(nfct_get_attr_u32(ct, ATTR_ORIG_NAT_SEQ_CORRECTION_POS)),
		.orig_seq_offset_before = 
		htonl(nfct_get_attr_u32(ct, ATTR_ORIG_NAT_SEQ_OFFSET_BEFORE)),
		.orig_seq_offset_after =
		htonl(nfct_get_attr_u32(ct, ATTR_ORIG_NAT_SEQ_OFFSET_AFTER)),
		.repl_seq_correction_pos = 
		htonl(nfct_get_attr_u32(ct, ATTR_REPL_NAT_SEQ_CORRECTION_POS)),
		.repl_seq_offset_before =
		htonl(nfct_get_attr_u32(ct, ATTR_REPL_NAT_SEQ_OFFSET_BEFORE)),
		.repl_seq_offset_after = 
		htonl(nfct_get_attr_u32(ct, ATTR_REPL_NAT_SEQ_OFFSET_AFTER))
	};
	addattr(n, NTA_NAT_SEQ_ADJ, &data, sizeof(struct nta_attr_natseqadj));
}
