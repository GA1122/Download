pgp_build_tlv(sc_context_t *ctx, unsigned int tag, u8 *data, size_t len, u8 **out, size_t *outlen)
{
	u8 highest_order = 0;
	int r;

	r = sc_asn1_write_element(ctx, tag, data, len, out, outlen);
	LOG_TEST_RET(ctx, r, "Failed to write ASN.1 element");

	 
	 
	while ((tag >> 8*highest_order) != 0) {
		highest_order++;
	}
	highest_order--;

	 
	if (highest_order < 4)
		*out[0] |= (tag >> 8*highest_order);

	return SC_SUCCESS;
}