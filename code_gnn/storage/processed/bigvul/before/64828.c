static boolean my_empty_output_buffer_fn(j_compress_ptr cinfo)
{
	struct iwjpegwcontext *wctx = (struct iwjpegwcontext*)cinfo->dest;

	(*wctx->iodescr->write_fn)(wctx->ctx,wctx->iodescr,
		wctx->buffer,wctx->buffer_len);
	wctx->pub.next_output_byte = wctx->buffer;
	wctx->pub.free_in_buffer = wctx->buffer_len;
	return TRUE;
}
