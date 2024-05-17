static void iwbmpr_misc_config(struct iw_context *ctx, struct iwbmprcontext *rctx)
{
	if(!rctx->topdown) {
		iw_reorient_image(ctx,IW_REORIENT_FLIP_V);
	}

	iw_set_input_colorspace(ctx,&rctx->csdescr);

	if(rctx->bitcount==16 || rctx->bitcount==32) {
		if(rctx->bf_bits_count[0]!=8 || rctx->bf_bits_count[1]!=8 || rctx->bf_bits_count[2]!=8 ||
			(IW_IMGTYPE_HAS_ALPHA(rctx->img->imgtype) && rctx->bf_bits_count[3]!=8))
		{
			iw_set_input_max_color_code(ctx,0, (1 << rctx->bf_bits_count[0])-1 );
			iw_set_input_max_color_code(ctx,1, (1 << rctx->bf_bits_count[1])-1 );
			iw_set_input_max_color_code(ctx,2, (1 << rctx->bf_bits_count[2])-1 );
			if(IW_IMGTYPE_HAS_ALPHA(rctx->img->imgtype)) {
				iw_set_input_max_color_code(ctx,3, (1 << rctx->bf_bits_count[3])-1 );
			}
		}
	}
}