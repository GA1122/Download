dissect_SEC_DESC_BUF(tvbuff_t *tvb, int offset, packet_info *pinfo,
		     proto_tree *tree, dcerpc_info *di, guint8 *drep)
{
	proto_tree *subtree;
	guint32 len;

	 

	subtree = proto_tree_add_subtree(
		tree, tvb, offset, 0, ett_SEC_DESC_BUF, NULL, "Security descriptor buffer");

	offset = dissect_ndr_uint32(
		tvb, offset, pinfo, subtree, di, drep,
		hf_secdescbuf_maxlen, NULL);

	offset = dissect_ndr_uint32(
		tvb, offset, pinfo, subtree, di, drep,
		hf_secdescbuf_undoc, NULL);

	offset = dissect_ndr_uint32(
		tvb, offset, pinfo, subtree, di, drep,
		hf_secdescbuf_len, &len);

	dissect_nt_sec_desc(
		tvb, offset, pinfo, subtree, drep, TRUE, len,
		&spoolss_printer_access_mask_info);

	offset += len;

	return offset;
}
