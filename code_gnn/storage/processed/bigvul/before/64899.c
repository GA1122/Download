static void decide_how_to_apply_bkgd(struct iw_context *ctx)
{
	if(!IW_IMGTYPE_HAS_ALPHA(ctx->img1_imgtype_logical)) {
		ctx->apply_bkgd=0;
		return;
	}

	if(ctx->img1_bkgd_label_set &&
		(ctx->req.use_bkgd_label_from_file || !ctx->req.bkgd_valid))
	{
		ctx->bkgd_color_source = IW_BKGD_COLOR_SOURCE_FILE;
	}
	else if(ctx->req.bkgd_valid) {
		ctx->bkgd_color_source = IW_BKGD_COLOR_SOURCE_REQ;
		ctx->bkgd_checkerboard = ctx->req.bkgd_checkerboard;
	}
	else {
		ctx->bkgd_color_source = IW_BKGD_COLOR_SOURCE_NONE;
	}

	if(ctx->bkgd_checkerboard) {
		if(ctx->bkgd_check_size<1) ctx->bkgd_check_size=1;
	}

	if(ctx->req.bkgd_valid) {
		ctx->apply_bkgd=1;
	}

	if(!(ctx->output_profile&IW_PROFILE_TRANSPARENCY)) {
		if(!ctx->req.bkgd_valid && !ctx->apply_bkgd) {
			iw_warning(ctx,"This image may have transparency, which is incompatible with the output format. A background color will be applied.");
		}
		ctx->apply_bkgd=1;
	}

	if(ctx->resize_settings[IW_DIMENSION_H].use_offset ||
		ctx->resize_settings[IW_DIMENSION_V].use_offset)
	{
		if(!ctx->req.bkgd_valid && !ctx->apply_bkgd) {
			iw_warning(ctx,"This image may have transparency, which is incompatible with a channel offset. A background color will be applied.");
		}
		ctx->apply_bkgd=1;

		if(ctx->bkgd_checkerboard && ctx->req.bkgd_checkerboard) {
			iw_warning(ctx,"Checkerboard backgrounds are not supported when using a channel offset.");
			ctx->bkgd_checkerboard=0;
		}
		ctx->apply_bkgd_strategy=IW_BKGD_STRATEGY_EARLY;
		return;
	}

	if(!ctx->apply_bkgd) {
		return;
	}

	if(ctx->bkgd_checkerboard) {
		ctx->apply_bkgd_strategy=IW_BKGD_STRATEGY_LATE;
		return;
	}

	ctx->apply_bkgd_strategy=IW_BKGD_STRATEGY_LATE;
}
