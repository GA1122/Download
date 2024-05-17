static int iwbmp_read_bits(struct iwbmprcontext *rctx)
{
	int retval = 0;

	rctx->img->width = rctx->width;
	rctx->img->height = rctx->height;

	if(rctx->fileheader_size>0) {
		size_t expected_offbits;

		expected_offbits = rctx->fileheader_size + rctx->infoheader_size +
			rctx->bitfields_nbytes + rctx->palette_nbytes;

		if(rctx->bfOffBits==expected_offbits) {
			;
		}
		else if(rctx->bfOffBits>expected_offbits && rctx->bfOffBits<1000000) {
			if(!iwbmp_skip_bytes(rctx, rctx->bfOffBits - expected_offbits)) goto done;
		}
		else {
			iw_set_error(rctx->ctx,"Invalid BMP bits offset");
			goto done;
		}
	}

	if(rctx->compression==IWBMP_BI_RGB) {
		if(!bmpr_read_uncompressed(rctx)) goto done;
	}
	else if(rctx->compression==IWBMP_BI_RLE8 || rctx->compression==IWBMP_BI_RLE4) {
		if(!bmpr_read_rle(rctx)) goto done;
	}
	else {
		iw_set_errorf(rctx->ctx,"Unsupported BMP compression or image type (%d)",(int)rctx->compression);
		goto done;
	}

	retval = 1;
done:
	return retval;
}