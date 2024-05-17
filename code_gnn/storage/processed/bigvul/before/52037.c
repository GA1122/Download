dissect_spoolss_buffer_data(tvbuff_t *tvb, int offset, packet_info *pinfo,
			    proto_tree *tree, dcerpc_info *di, guint8 *drep)
{
	BUFFER *b = (BUFFER *)di->private_data;
	proto_item *item;
	guint32 size;
	const guint8 *data;

	if (di->conformant_run)
		return offset;

	 

	offset = dissect_ndr_uint32(tvb, offset, pinfo, tree, di, drep,
				    hf_buffer_size, &size);

	offset = dissect_ndr_uint8s(tvb, offset, pinfo, NULL, di, drep,
				    hf_buffer_data, size, &data);

	item = proto_tree_add_item(
		tree, hf_buffer_data, tvb, offset - size,
		size, ENC_NA);

	 

	if (b) {

		 

		b->tvb = tvb_new_child_real_data(tvb, data, size, size);
		add_new_data_source(pinfo, b->tvb, "SPOOLSS buffer");

		b->item = item;
		b->tree = proto_item_add_subtree(item, ett_BUFFER);
	}

	return offset;
}
