static int asn1_decode_path(sc_context_t *ctx, const u8 *in, size_t len,
			    sc_path_t *path, int depth)
{
	int idx, count, r;
	struct sc_asn1_entry asn1_path_ext[3], asn1_path[5];
	unsigned char path_value[SC_MAX_PATH_SIZE], aid_value[SC_MAX_AID_SIZE];
	size_t path_len = sizeof(path_value), aid_len = sizeof(aid_value);

	memset(path, 0, sizeof(struct sc_path));

	sc_copy_asn1_entry(c_asn1_path_ext, asn1_path_ext);
	sc_copy_asn1_entry(c_asn1_path, asn1_path);

	sc_format_asn1_entry(asn1_path_ext + 0, aid_value, &aid_len, 0);
	sc_format_asn1_entry(asn1_path_ext + 1, path_value, &path_len, 0);

	sc_format_asn1_entry(asn1_path + 0, path_value, &path_len, 0);
	sc_format_asn1_entry(asn1_path + 1, &idx, NULL, 0);
	sc_format_asn1_entry(asn1_path + 2, &count, NULL, 0);
	sc_format_asn1_entry(asn1_path + 3, asn1_path_ext, NULL, 0);

	r = asn1_decode(ctx, asn1_path, in, len, NULL, NULL, 0, depth + 1);
	if (r)
		return r;

	if (asn1_path[3].flags & SC_ASN1_PRESENT)   {
		 
		memcpy(path->aid.value, aid_value, aid_len);
		path->aid.len = aid_len;

		memcpy(path->value, path_value, path_len);
		path->len = path_len;
	}
	else if (asn1_path[0].flags & SC_ASN1_PRESENT)   {
		 
		memcpy(path->value, path_value, path_len);
		path->len = path_len;
	}
	else   {
		 
		return SC_ERROR_ASN1_OBJECT_NOT_FOUND;
	}

	if (path->len == 2)
		path->type = SC_PATH_TYPE_FILE_ID;
	else   if (path->aid.len && path->len > 2)
		path->type = SC_PATH_TYPE_FROM_CURRENT;
	else
		path->type = SC_PATH_TYPE_PATH;

	if ((asn1_path[1].flags & SC_ASN1_PRESENT) && (asn1_path[2].flags & SC_ASN1_PRESENT)) {
		path->index = idx;
		path->count = count;
	}
	else {
		path->index = 0;
		path->count = -1;
	}

	return SC_SUCCESS;
}