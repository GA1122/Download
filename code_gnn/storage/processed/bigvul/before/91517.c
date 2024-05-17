void jpeg_gdIOCtx_dest (j_compress_ptr cinfo, gdIOCtx * outfile)
{
	my_dest_ptr dest;

	 
	if (cinfo->dest == NULL) {  
		cinfo->dest = (struct jpeg_destination_mgr *) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof (my_destination_mgr));
	}

	dest = (my_dest_ptr) cinfo->dest;
	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;
	dest->outfile = outfile;
}
