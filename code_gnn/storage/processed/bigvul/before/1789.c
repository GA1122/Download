unpack_bz2_stream(transformer_state_t *xstate)
{
	IF_DESKTOP(long long total_written = 0;)
	bunzip_data *bd;
	char *outbuf;
	int i;
	unsigned len;

	if (check_signature16(xstate, BZIP2_MAGIC))
		return -1;

	outbuf = xmalloc(IOBUF_SIZE);
	len = 0;
	while (1) {  

		i = start_bunzip(&bd, xstate->src_fd, outbuf + 2, len);

		if (i == 0) {
			while (1) {  
				i = read_bunzip(bd, outbuf, IOBUF_SIZE);
				if (i < 0)  
					break;
				i = IOBUF_SIZE - i;  
				if (i == 0)  
					break;
				if (i != transformer_write(xstate, outbuf, i)) {
					i = RETVAL_SHORT_WRITE;
					goto release_mem;
				}
				IF_DESKTOP(total_written += i;)
			}
		}

		if (i != RETVAL_LAST_BLOCK
		 
		 && i != RETVAL_OK
		) {
			bb_error_msg("bunzip error %d", i);
			break;
		}
		if (bd->headerCRC != bd->totalCRC) {
			bb_error_msg("CRC error");
			break;
		}

		 
		i = RETVAL_OK;

		 
		len = bd->inbufCount - bd->inbufPos;
		memcpy(outbuf, &bd->inbuf[bd->inbufPos], len);
		if (len < 2) {
			if (safe_read(xstate->src_fd, outbuf + len, 2 - len) != 2 - len)
				break;
			len = 2;
		}
		if (*(uint16_t*)outbuf != BZIP2_MAGIC)  
			break;
		dealloc_bunzip(bd);
		len -= 2;
	}

 release_mem:
	dealloc_bunzip(bd);
	free(outbuf);

	return i ? i : IF_DESKTOP(total_written) + 0;
}
