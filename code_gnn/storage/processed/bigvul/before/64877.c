static int iwbmp_write_main(struct iwbmpwcontext *wctx)
{
	struct iw_image *img;
	int cmpr_req;
	int retval = 0;
	int x;
	const char *optv;

	img = wctx->img;

	wctx->bmpversion = 0;
	optv = iw_get_option(wctx->ctx, "bmp:version");
	if(optv) {
		wctx->bmpversion = iw_parse_int(optv);
	}

	if(wctx->bmpversion==0) wctx->bmpversion=3;
	if(wctx->bmpversion==4) {
		iw_warning(wctx->ctx,"Writing BMP v4 is not supported; using v3 instead");
		wctx->bmpversion=3;
	}
	if(wctx->bmpversion!=2 && wctx->bmpversion!=3 && wctx->bmpversion!=5) {
		iw_set_errorf(wctx->ctx,"Unsupported BMP version: %d",wctx->bmpversion);
		goto done;
	}

	if(wctx->bmpversion>=3)
		cmpr_req = iw_get_value(wctx->ctx,IW_VAL_COMPRESSION);
	else
		cmpr_req = IW_COMPRESSION_NONE;

	if(wctx->bmpversion==2)
		wctx->header_size = 12;
	else if(wctx->bmpversion==5)
		wctx->header_size = 124;
	else
		wctx->header_size = 40;

	wctx->no_cslabel = iw_get_value(wctx->ctx,IW_VAL_NO_CSLABEL);

	if(cmpr_req==IW_COMPRESSION_AUTO || cmpr_req==IW_COMPRESSION_NONE)
		cmpr_req = IW_COMPRESSION_NONE;
	else
		cmpr_req = IW_COMPRESSION_RLE;

	if(img->imgtype==IW_IMGTYPE_RGB) {
		if(img->reduced_maxcolors) {
			if(!setup_16_32bit(wctx,img->maxcolorcode[IW_CHANNELTYPE_RED],
				img->maxcolorcode[IW_CHANNELTYPE_GREEN],
				img->maxcolorcode[IW_CHANNELTYPE_BLUE],0))
			{
				goto done;
			}
		}
		else {
			wctx->bitcount=24;
		}
	}
	else if(img->imgtype==IW_IMGTYPE_PALETTE) {
		if(!wctx->pal) goto done;

		x = check_palette_transparency(wctx->pal);

		if(x!=0 && wctx->bmpversion<3) {
			iw_set_error(wctx->ctx,"Cannot save as a transparent BMP: Incompatible BMP version");
			goto done;
		}
		else if(x==2) {
			iw_set_error(wctx->ctx,"Cannot save this image as a transparent BMP: Has partial transparency");
			goto done;
		}
		else if(x!=0 && cmpr_req!=IW_COMPRESSION_RLE) {
			iw_set_error(wctx->ctx,"Cannot save as a transparent BMP: RLE compression required");
			goto done;
		}

		if(wctx->pal->num_entries<=2 && cmpr_req!=IW_COMPRESSION_RLE)
			wctx->bitcount=1;
		else if(wctx->pal->num_entries<=16)
			wctx->bitcount=4;
		else
			wctx->bitcount=8;
	}
	else if(img->imgtype==IW_IMGTYPE_RGBA) {
		if(img->reduced_maxcolors) {
			if(!setup_16_32bit(wctx,img->maxcolorcode[IW_CHANNELTYPE_RED],
				img->maxcolorcode[IW_CHANNELTYPE_GREEN],
				img->maxcolorcode[IW_CHANNELTYPE_BLUE],
				img->maxcolorcode[IW_CHANNELTYPE_ALPHA]))
			{
				goto done;
			}
		}
		else {
			if(!setup_16_32bit(wctx,255,255,255,255)) {
				goto done;
			}
		}
	}
	else if(img->imgtype==IW_IMGTYPE_GRAYA) {
		if(img->reduced_maxcolors) {
			if(!setup_16_32bit(wctx,img->maxcolorcode[IW_CHANNELTYPE_GRAY],
				img->maxcolorcode[IW_CHANNELTYPE_GRAY],
				img->maxcolorcode[IW_CHANNELTYPE_GRAY],
				img->maxcolorcode[IW_CHANNELTYPE_ALPHA]))
			{
				goto done;
			}
		}
		else {
			if(!setup_16_32bit(wctx,255,255,255,255)) {
				goto done;
			}
		}
	}
	else if(img->imgtype==IW_IMGTYPE_GRAY) {
		if(img->reduced_maxcolors) {
			if(img->maxcolorcode[IW_CHANNELTYPE_GRAY]<=1023) {
				if(!setup_16_32bit(wctx,img->maxcolorcode[IW_CHANNELTYPE_GRAY],
					img->maxcolorcode[IW_CHANNELTYPE_GRAY],
					img->maxcolorcode[IW_CHANNELTYPE_GRAY],0))
				{
					goto done;
				}
			}
			else {
				iw_set_error(wctx->ctx,"Cannot write grayscale BMP at this bit depth");
				goto done;
			}
		}
		else {
			wctx->bitcount=24;
		}
	}
	else {
		iw_set_error(wctx->ctx,"Internal: Bad image type for BMP");
		goto done;
	}

	if(cmpr_req==IW_COMPRESSION_RLE && (wctx->bitcount==4 || wctx->bitcount==8)) {
		wctx->compressed = 1;
	}

	wctx->unc_dst_bpr = iwbmp_calc_bpr(wctx->bitcount,img->width);
	wctx->unc_bitssize = wctx->unc_dst_bpr * img->height;
	wctx->palentries = 0;

	if(wctx->pal) {
		if(wctx->bmpversion==2) {
			wctx->palentries = 1<<wctx->bitcount;
			wctx->palsize = wctx->palentries*3;
		}
		else {
			if(wctx->bitcount==1) {
				wctx->palentries=2;
			}
			else {
				wctx->palentries = wctx->pal->num_entries;
			}
			wctx->palsize = wctx->palentries*4;
		}
	}

	iwbmp_write_file_header(wctx);

	if(!iwbmp_write_bmp_header(wctx)) {
		goto done;
	}

	if(wctx->bitfields_size>0) {
		if(!iwbmp_write_bitfields(wctx)) goto done;
	}

	iwbmp_write_palette(wctx);

	if(wctx->compressed) {
		if(!iwbmp_write_pixels_compressed(wctx,img)) goto done;
	}
	else {
		iwbmp_write_pixels_uncompressed(wctx,img);
	}

	retval = 1;
done:
	return retval;
}