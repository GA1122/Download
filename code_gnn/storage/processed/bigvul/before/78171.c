static int asepcos_tlvpath_to_scpath(sc_path_t *out, const u8 *in, size_t in_len)
{
	int    r;
	size_t len = in_len;

	memset(out, 0, sizeof(sc_path_t));

	while (len != 0) {
		if (len < 4)
			return SC_ERROR_INTERNAL;
		if (in[0] != 0x8b || in[1] != 0x02)
			return SC_ERROR_INVALID_ASN1_OBJECT;
		 
		r = sc_append_path_id(out, &in[2], 2);
		if (r != SC_SUCCESS)
			return r;
		len -= 4;
		in  += 4;
	}
	out->type = SC_PATH_TYPE_PATH;

	return SC_SUCCESS;
}