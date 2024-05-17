static int iwbmp_write_pixels_compressed(struct iwbmpwcontext *wctx,
	struct iw_image *img)
{
	struct rle_context rlectx;
	int j;
	int retval = 0;

	iw_zeromem(&rlectx,sizeof(struct rle_context));

	rlectx.ctx = wctx->ctx;
	rlectx.wctx = wctx;
	rlectx.total_bytes_written = 0;
	rlectx.img_width = img->width;

	for(j=img->height-1;j>=0;j--) {
		rlectx.srcrow = &img->pixels[j*img->bpr];
		rlectx.cur_row = j;

		if(wctx->bitcount==4) {
			if(!rle4_compress_row(&rlectx)) goto done;
		}
		else if(wctx->bitcount==8) {
			if(!rle8_compress_row(&rlectx)) goto done;
		}
		else {
			goto done;
		}
	}

	if(!rle_patch_file_size(wctx,rlectx.total_bytes_written)) goto done;

	retval = 1;
done:
	return retval;
}