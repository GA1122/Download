static int iw_process_cols_to_intermediate(struct iw_context *ctx, int channel,
	const struct iw_csdescr *in_csdescr)
{
	int i,j;
	int retval=0;
	iw_tmpsample tmp_alpha;
	iw_tmpsample *inpix_tofree = NULL;
	iw_tmpsample *outpix_tofree = NULL;
	int is_alpha_channel;
	struct iw_resize_settings *rs = NULL;
	struct iw_channelinfo_intermed *int_ci;

	iw_tmpsample *in_pix;
	iw_tmpsample *out_pix;
	int num_in_pix;
	int num_out_pix;

	int_ci = &ctx->intermed_ci[channel];
	is_alpha_channel = (int_ci->channeltype==IW_CHANNELTYPE_ALPHA);

	num_in_pix = ctx->input_h;
	inpix_tofree = (iw_tmpsample*)iw_malloc(ctx, num_in_pix * sizeof(iw_tmpsample));
	if(!inpix_tofree) goto done;
	in_pix = inpix_tofree;

	num_out_pix = ctx->intermed_canvas_height;
	outpix_tofree = (iw_tmpsample*)iw_malloc(ctx, num_out_pix * sizeof(iw_tmpsample));
	if(!outpix_tofree) goto done;
	out_pix = outpix_tofree;

	rs=&ctx->resize_settings[IW_DIMENSION_V];

	if(!rs->rrctx) {
		rs->rrctx = iwpvt_resize_rows_init(ctx,rs,int_ci->channeltype,
			num_in_pix, num_out_pix);
		if(!rs->rrctx) goto done;
	}

	for(i=0;i<ctx->input_w;i++) {

		for(j=0;j<ctx->input_h;j++) {

			in_pix[j] = get_sample_cvt_to_linear(ctx,i,j,channel,in_csdescr);

			if(int_ci->need_unassoc_alpha_processing) {  
				tmp_alpha = get_raw_sample(ctx,i,j,ctx->img1_alpha_channel_index);

				in_pix[j] *= tmp_alpha;
			}
			else if(ctx->apply_bkgd && ctx->apply_bkgd_strategy==IW_BKGD_STRATEGY_EARLY) {
				tmp_alpha = get_raw_sample(ctx,i,j,ctx->img1_alpha_channel_index);
				in_pix[j] = (tmp_alpha)*(in_pix[j]) +
					(1.0-tmp_alpha)*(int_ci->bkgd_color_lin);
			}
		}


		iwpvt_resize_row_main(rs->rrctx,in_pix,out_pix);

		if(ctx->intclamp)
			clamp_output_samples(ctx,out_pix,num_out_pix);

		for(j=0;j<ctx->intermed_canvas_height;j++) {
			if(is_alpha_channel) {
				ctx->intermediate_alpha32[((size_t)j)*ctx->intermed_canvas_width + i] = (iw_float32)out_pix[j];
			}
			else {
				ctx->intermediate32[((size_t)j)*ctx->intermed_canvas_width + i] = (iw_float32)out_pix[j];
			}
		}
	}

	retval=1;

done:
	if(rs && rs->disable_rrctx_cache && rs->rrctx) {
		iwpvt_resize_rows_done(rs->rrctx);
		rs->rrctx = NULL;
	}
	if(inpix_tofree) iw_free(ctx,inpix_tofree);
	if(outpix_tofree) iw_free(ctx,outpix_tofree);
 	return retval;
 }