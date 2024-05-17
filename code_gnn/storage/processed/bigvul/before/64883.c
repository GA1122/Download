static int process_bf_mask(struct iwbmprcontext *rctx, int k)
{
	rctx->bf_high_bit[k] = find_high_bit(rctx->bf_mask[k]);
	rctx->bf_low_bit[k] = find_low_bit(rctx->bf_mask[k]);
	rctx->bf_bits_count[k] = 1+rctx->bf_high_bit[k]-rctx->bf_low_bit[k];

	if(rctx->bf_high_bit[k] > (int)(rctx->bitcount-1)) return 0;

	if(rctx->bf_bits_count[k]>16) {
		rctx->bf_low_bit[k] = rctx->bf_high_bit[k]-15;
		rctx->bf_bits_count[k] = 16;
	}

	if(rctx->bf_bits_count[k]>8) {
		rctx->need_16bit = 1;
	}

	return 1;
}
