static int iw_process_internal(struct iw_context *ctx)
{
	int channel;
	int retval=0;
	int i,k;
	int ret;
	struct iw_csdescr csdescr_linear;

	ctx->intermediate32=NULL;
	ctx->intermediate_alpha32=NULL;
	ctx->final_alpha32=NULL;
	ctx->intermed_canvas_width = ctx->input_w;
	ctx->intermed_canvas_height = ctx->img2.height;

	iw_make_linear_csdescr(&csdescr_linear);

	ctx->img2.bpr = iw_calc_bytesperrow(ctx->img2.width,ctx->img2.bit_depth*ctx->img2_numchannels);

	ctx->img2.pixels = iw_malloc_large(ctx, ctx->img2.bpr, ctx->img2.height);
	if(!ctx->img2.pixels) {
		goto done;
	}

	ctx->intermediate32 = (iw_float32*)iw_malloc_large(ctx, ctx->intermed_canvas_width * ctx->intermed_canvas_height, sizeof(iw_float32));
	if(!ctx->intermediate32) {
		goto done;
	}

	if(ctx->uses_errdiffdither) {
		for(k=0;k<IW_DITHER_MAXROWS;k++) {
			ctx->dither_errors[k] = (double*)iw_malloc(ctx, ctx->img2.width * sizeof(double));
			if(!ctx->dither_errors[k]) goto done;
		}
	}

	if(!ctx->disable_output_lookup_tables) {
		iw_make_x_to_linear_table(ctx,&ctx->output_rev_color_corr_table,&ctx->img2,&ctx->img2cs);

		iw_make_nearest_color_table(ctx,&ctx->nearest_color_table,&ctx->img2,&ctx->img2cs);
	}

	if(IW_IMGTYPE_HAS_ALPHA(ctx->intermed_imgtype)) {
		ctx->intermediate_alpha32 = (iw_float32*)iw_malloc_large(ctx, ctx->intermed_canvas_width * ctx->intermed_canvas_height, sizeof(iw_float32));
		if(!ctx->intermediate_alpha32) {
			goto done;
		}
		ctx->final_alpha32 = (iw_float32*)iw_malloc_large(ctx, ctx->img2.width * ctx->img2.height, sizeof(iw_float32));
		if(!ctx->final_alpha32) {
			goto done;
		}

		if(!iw_process_one_channel(ctx,ctx->intermed_alpha_channel_index,&csdescr_linear,&csdescr_linear)) goto done;
	}


	for(channel=0;channel<ctx->intermed_numchannels;channel++) {
		if(ctx->intermed_ci[channel].channeltype!=IW_CHANNELTYPE_ALPHA) {
			if(ctx->no_gamma)
				ret=iw_process_one_channel(ctx,channel,&csdescr_linear,&csdescr_linear);
			else
				ret=iw_process_one_channel(ctx,channel,&ctx->img1cs,&ctx->img2cs);

			if(!ret) goto done;
		}
	}

	iw_process_bkgd_label(ctx);

	if(ctx->req.negate_target) {
		negate_target_image(ctx);
	}

	retval=1;

done:
	if(ctx->intermediate32) { iw_free(ctx,ctx->intermediate32); ctx->intermediate32=NULL; }
	if(ctx->intermediate_alpha32) { iw_free(ctx,ctx->intermediate_alpha32); ctx->intermediate_alpha32=NULL; }
	if(ctx->final_alpha32) { iw_free(ctx,ctx->final_alpha32); ctx->final_alpha32=NULL; }
	for(k=0;k<IW_DITHER_MAXROWS;k++) {
		if(ctx->dither_errors[k]) { iw_free(ctx,ctx->dither_errors[k]); ctx->dither_errors[k]=NULL; }
	}
	for(i=0;i<2;i++) {  
		if(ctx->resize_settings[i].rrctx) {
			iwpvt_resize_rows_done(ctx->resize_settings[i].rrctx);
			ctx->resize_settings[i].rrctx = NULL;
		}
	}
	return retval;
}