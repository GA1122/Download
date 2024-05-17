static bool encode_flag_request(void *mem_ctx, void *in, DATA_BLOB *out)
{
	if (in) {
		return false;
	}

	*out = data_blob(NULL, 0);
	return true;
}
