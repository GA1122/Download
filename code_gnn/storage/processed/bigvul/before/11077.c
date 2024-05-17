static bool smb_splice_chain(uint8_t **poutbuf, uint8_t smb_command,
			     uint8_t wct, const uint16_t *vwv,
			     size_t bytes_alignment,
			     uint32_t num_bytes, const uint8_t *bytes)
{
	uint8_t *outbuf;
	size_t old_size, new_size;
	size_t ofs;
	size_t chain_padding = 0;
	size_t bytes_padding = 0;
	bool first_request;

	old_size = talloc_get_size(*poutbuf);

	 

	first_request = (old_size == smb_wct);

	if (!first_request && ((old_size % 4) != 0)) {
		 
		chain_padding = 4 - (old_size % 4);
	}

	 

	new_size = old_size + chain_padding + 1 + wct * sizeof(uint16_t) + 2;

	if ((bytes_alignment != 0) && ((new_size % bytes_alignment) != 0)) {
		bytes_padding = bytes_alignment - (new_size % bytes_alignment);
	}

	new_size += bytes_padding + num_bytes;

	if ((smb_command != SMBwriteX) && (new_size > 0xffff)) {
		DEBUG(1, ("splice_chain: %u bytes won't fit\n",
			  (unsigned)new_size));
		return false;
	}

	outbuf = TALLOC_REALLOC_ARRAY(NULL, *poutbuf, uint8_t, new_size);
	if (outbuf == NULL) {
		DEBUG(0, ("talloc failed\n"));
		return false;
	}
	*poutbuf = outbuf;

	if (first_request) {
		SCVAL(outbuf, smb_com, smb_command);
	} else {
		size_t andx_cmd_ofs;

		if (!find_andx_cmd_ofs(outbuf, &andx_cmd_ofs)) {
			DEBUG(1, ("invalid command chain\n"));
			*poutbuf = TALLOC_REALLOC_ARRAY(
				NULL, *poutbuf, uint8_t, old_size);
			return false;
		}

		if (chain_padding != 0) {
			memset(outbuf + old_size, 0, chain_padding);
			old_size += chain_padding;
		}

		SCVAL(outbuf, andx_cmd_ofs, smb_command);
		SSVAL(outbuf, andx_cmd_ofs + 2, old_size - 4);
	}

	ofs = old_size;

	 

	SCVAL(outbuf, ofs, wct);
	ofs += 1;

	 

	memcpy(outbuf + ofs, vwv, sizeof(uint16_t) * wct);
	ofs += sizeof(uint16_t) * wct;

	 

	SSVAL(outbuf, ofs, num_bytes + bytes_padding);
	ofs += sizeof(uint16_t);

	 

	if (bytes_padding != 0) {
		memset(outbuf + ofs, 0, bytes_padding);
		ofs += bytes_padding;
	}

	 

	memcpy(outbuf + ofs, bytes, num_bytes);

	return true;
}