static int decode_v3_header_fields(struct iwbmprcontext *rctx, const iw_byte *buf)
{
	unsigned int nplanes;
	int biXPelsPerMeter, biYPelsPerMeter;
	unsigned int biClrUsed = 0;

	rctx->width = iw_get_i32le(&buf[4]);
	rctx->height = iw_get_i32le(&buf[8]);
	if(rctx->height<0) {
		rctx->height = -rctx->height;
		rctx->topdown = 1;
	}

	nplanes = iw_get_ui16le(&buf[12]);
	if(nplanes!=1) return 0;

	rctx->bitcount = iw_get_ui16le(&buf[14]);
	if(rctx->bitcount!=1 && rctx->bitcount!=2 && rctx->bitcount!=4 &&
		rctx->bitcount!=8 && rctx->bitcount!=16 && rctx->bitcount!=24 &&
		rctx->bitcount!=32)
	{
		iw_set_errorf(rctx->ctx,"Bad or unsupported bit count (%d)",(int)rctx->bitcount);
		return 0;
	}

	if(rctx->infoheader_size<=16) {
		goto infoheaderdone;
	}

	rctx->compression = iw_get_ui32le(&buf[16]);
	if(rctx->compression==IWBMP_BI_BITFIELDS) {
		if(rctx->bitcount==1) {
			iw_set_error(rctx->ctx,"Huffman 1D compression not supported");
			return 0;
		}
		else if(rctx->bitcount!=16 && rctx->bitcount!=32) {
			iw_set_error(rctx->ctx,"Bad or unsupported image type");
			return 0;
		}

		rctx->uses_bitfields = 1;

		if(rctx->bmpversion==3) {
			rctx->bitfields_nbytes = 12;
		}

		rctx->compression=IWBMP_BI_RGB;
	}

	biXPelsPerMeter = iw_get_i32le(&buf[24]);
	biYPelsPerMeter = iw_get_i32le(&buf[28]);

	rctx->img->density_code = IW_DENSITY_UNITS_PER_METER;
	rctx->img->density_x = (double)biXPelsPerMeter;
	rctx->img->density_y = (double)biYPelsPerMeter;
	if(!iw_is_valid_density(rctx->img->density_x,rctx->img->density_y,rctx->img->density_code)) {
		rctx->img->density_code=IW_DENSITY_UNKNOWN;
	}

	biClrUsed = iw_get_ui32le(&buf[32]);
	if(biClrUsed>100000) return 0;

infoheaderdone:
	if(biClrUsed==0 && rctx->bitcount<=8) {
		rctx->palette_entries = 1<<rctx->bitcount;
	}
	else {
		rctx->palette_entries = biClrUsed;
	}
	rctx->palette_nbytes = 4*rctx->palette_entries;
	return 1;
}