static bool decode_flag_request(void *mem_ctx, DATA_BLOB in, void *_out)
{
	if (in.length != 0) {
		return false;
	}

	return true;
}
