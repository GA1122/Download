void jpeg_gdIOCtx_src (j_decompress_ptr cinfo, gdIOCtx * infile)
{
	my_src_ptr src;

	 
	if (cinfo->src == NULL) {  
		cinfo->src = (struct jpeg_source_mgr *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof (my_source_mgr));
		src = (my_src_ptr) cinfo->src;
		src->buffer = (unsigned char *) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, INPUT_BUF_SIZE * sizeof (unsigned char));

	}

	src = (my_src_ptr) cinfo->src;
	src->pub.init_source = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart;	 
	src->pub.term_source = term_source;
	src->infile = infile;
	src->pub.bytes_in_buffer = 0;	 
	src->pub.next_input_byte = NULL;	 
}
