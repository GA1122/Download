static void my_init_destination_fn(j_compress_ptr cinfo)
{
	struct iwjpegwcontext *wctx = (struct iwjpegwcontext*)cinfo->dest;

	wctx->pub.next_output_byte = wctx->buffer;
	wctx->pub.free_in_buffer = wctx->buffer_len;
}
