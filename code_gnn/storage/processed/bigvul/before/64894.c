static int setup_16_32bit(struct iwbmpwcontext *wctx,
	int mcc_r, int mcc_g, int mcc_b, int mcc_a)
{
	int has_alpha;

	has_alpha = IW_IMGTYPE_HAS_ALPHA(wctx->img->imgtype);

	if(wctx->bmpversion<3) {
		iw_set_errorf(wctx->ctx,"Bit depth incompatible with BMP version %d",
			wctx->bmpversion);
		return 0;
	}

	if(has_alpha && wctx->bmpversion<5) {
		iw_set_error(wctx->ctx,"Internal: Attempt to write v3 16- or 32-bit image with transparency");
		return 0;
	}

	wctx->maxcolor[0] = mcc_r;
	wctx->maxcolor[1] = mcc_g;
	wctx->maxcolor[2] = mcc_b;
	if(has_alpha) wctx->maxcolor[3] = mcc_a;

	if(!iwbmp_calc_bitfields_masks(wctx,has_alpha?4:3)) return 0;

	if(mcc_r==31 && mcc_g==31 && mcc_b==31 && !has_alpha) {
		wctx->bitfields_size = 0;
	}
	else {
		wctx->uses_bitfields = 1;
		wctx->bitfields_size = (wctx->bmpversion==3) ? 12 : 0;
	}
	return 1;
}