static int iwbmp_calc_bitfields_masks(struct iwbmpwcontext *wctx, int num_masks)
{
	int k;
	int bits[4];  
	int tot_bits = 0;

	for(k=0;k<num_masks;k++) {
		bits[k] = iw_max_color_to_bitdepth(wctx->maxcolor[k]);
		tot_bits += bits[k];
	}

	if(tot_bits > 32) {
		iw_set_error(wctx->ctx,"Cannot write a BMP image in this color format");
		return 0;
	}
	
	wctx->bitcount = (tot_bits>16) ? 32 : 16;

	wctx->bf_amt_to_shift[0] = bits[1] + bits[2];
	wctx->bf_amt_to_shift[1] = bits[2];
	wctx->bf_amt_to_shift[2] = 0;
	if(num_masks>3) wctx->bf_amt_to_shift[3] =  bits[0] + bits[1] + bits[2];

	for(k=0;k<num_masks;k++) {
		wctx->bf_mask[k] = wctx->maxcolor[k] << wctx->bf_amt_to_shift[k];
	}

	return 1;
}