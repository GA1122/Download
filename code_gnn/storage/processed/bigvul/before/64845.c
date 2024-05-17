static int bmpr_read_uncompressed(struct iwbmprcontext *rctx)
{
	iw_byte *rowbuf = NULL;
	size_t bmp_bpr;
	int j;
	int retval = 0;

	if(rctx->has_alpha_channel) {
		rctx->img->imgtype = IW_IMGTYPE_RGBA;
		
		rctx->img->bit_depth = rctx->need_16bit ? 16 : 8;
		rctx->img->bpr = iw_calc_bytesperrow(rctx->width,4*rctx->img->bit_depth);
	}
	else {
		rctx->img->imgtype = IW_IMGTYPE_RGB;
		rctx->img->bit_depth = rctx->need_16bit ? 16 : 8;
		rctx->img->bpr = iw_calc_bytesperrow(rctx->width,3*rctx->img->bit_depth);
	}

	bmp_bpr = iwbmp_calc_bpr(rctx->bitcount,rctx->width);

	rctx->img->pixels = (iw_byte*)iw_malloc_large(rctx->ctx,rctx->img->bpr,rctx->img->height);
	if(!rctx->img->pixels) goto done;

	rowbuf = iw_malloc(rctx->ctx,bmp_bpr);

	for(j=0;j<rctx->img->height;j++) {
		if(!iwbmp_read(rctx,rowbuf,bmp_bpr)) {
			goto done;
		}
		switch(rctx->bitcount) {
		case 32:
		case 16:
			bmpr_convert_row_32_16(rctx,rowbuf,j);
			break;
		case 24:
			bmpr_convert_row_24(rctx,rowbuf,j);
			break;
		case 8:
			bmpr_convert_row_8(rctx,rowbuf,j);
			break;
		case 4:
			bmpr_convert_row_4(rctx,rowbuf,j);
			break;
		case 2:
			bmpr_convert_row_2(rctx,rowbuf,j);
			break;
		case 1:
			bmpr_convert_row_1(rctx,rowbuf,j);
			break;
		}
	}

	retval = 1;
done:
	if(rowbuf) iw_free(rctx->ctx,rowbuf);
	return retval;
}