dissect_spoolss_relstr(tvbuff_t *tvb, int offset, packet_info *pinfo,
		       proto_tree *tree, dcerpc_info *di, guint8 *drep, int hf_index,
		       int struct_start, char **data)
{
	proto_item *item;
	proto_tree *subtree;
	guint32 relstr_offset, relstr_start, relstr_end;
	char *text;

	 

	offset = dissect_ndr_uint32(
		tvb, offset, pinfo, NULL, di, drep, hf_offset, &relstr_offset);

	relstr_start = relstr_offset + struct_start;

	if (relstr_offset) {
		relstr_end = dissect_spoolss_uint16uni(
			tvb, relstr_start, pinfo, NULL, drep, &text, hf_relative_string);
	} else { 			 
		text = g_strdup("");
		relstr_end = relstr_start;
	}

	 

	item = proto_tree_add_string(tree, hf_index, tvb, relstr_start, relstr_end - relstr_start, text);
	subtree = proto_item_add_subtree(item, ett_RELSTR);

	dissect_ndr_uint32(
		tvb, offset - 4, pinfo, subtree, di, drep, hf_offset, NULL);

	if (relstr_offset)
		dissect_spoolss_uint16uni(
			tvb, relstr_start, pinfo, subtree, drep, NULL, hf_relative_string);

	if (data)
		*data = text;
	else
		g_free(text);

	return offset;
}
