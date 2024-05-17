safeboolean fill_input_buffer (j_decompress_ptr cinfo)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;
	 
	ssize_t nbytes = 0;

	 
	 
	memset(src->buffer, 0, INPUT_BUF_SIZE);

	while (nbytes < INPUT_BUF_SIZE) {
		int got = gdGetBuf(src->buffer + nbytes, INPUT_BUF_SIZE - nbytes, src->infile);

		if (got == EOF || got == 0) {
			 
			if (!nbytes) {
				nbytes = -1;
			}
			break;
		}
		nbytes += got;
	}

	if (nbytes <= 0) {
		if (src->start_of_file)	{  
			ERREXIT (cinfo, JERR_INPUT_EMPTY);
		}
		WARNMS (cinfo, JWRN_JPEG_EOF);
		 
		src->buffer[0] = (unsigned char) 0xFF;
		src->buffer[1] = (unsigned char) JPEG_EOI;
		nbytes = 2;
	}

	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file = FALSE;

	return TRUE;
}