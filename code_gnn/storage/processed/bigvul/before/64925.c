static int iw_prepare_processing(struct iw_context *ctx, int w, int h)
{
	int i,j;
	int output_maxcolorcode_int;
	int strategy1, strategy2;
	int flag;

	if(ctx->output_profile==0) {
		iw_set_error(ctx,"Output profile not set");
		return 0;
	}

	if(!ctx->prng) {
		ctx->prng = iwpvt_prng_create(ctx);
	}

	if(ctx->randomize) {
		ctx->random_seed = iwpvt_util_randomize(ctx->prng);
	}

	if(ctx->req.out_true_valid) {
		ctx->resize_settings[IW_DIMENSION_H].out_true_size = ctx->req.out_true_width;
		ctx->resize_settings[IW_DIMENSION_V].out_true_size = ctx->req.out_true_height;
	}
	else {
		ctx->resize_settings[IW_DIMENSION_H].out_true_size = (double)w;
		ctx->resize_settings[IW_DIMENSION_V].out_true_size = (double)h;
	}

	if(!iw_check_image_dimensions(ctx,ctx->img1.width,ctx->img1.height)) {
		return 0;
	}
	if(!iw_check_image_dimensions(ctx,w,h)) {
		return 0;
	}

	if(ctx->to_grayscale) {
		prepare_grayscale(ctx);
	}

	init_channel_info(ctx);

	ctx->img2.width = w;
	ctx->img2.height = h;

	if(ctx->input_start_x<0) ctx->input_start_x=0;
	if(ctx->input_start_y<0) ctx->input_start_y=0;
	if(ctx->input_start_x>ctx->img1.width-1) ctx->input_start_x=ctx->img1.width-1;
	if(ctx->input_start_y>ctx->img1.height-1) ctx->input_start_x=ctx->img1.height-1;
	if(ctx->input_w<0) ctx->input_w = ctx->img1.width - ctx->input_start_x;
	if(ctx->input_h<0) ctx->input_h = ctx->img1.height - ctx->input_start_y;
	if(ctx->input_w<1) ctx->input_w = 1;
	if(ctx->input_h<1) ctx->input_h = 1;
	if(ctx->input_w>(ctx->img1.width-ctx->input_start_x)) ctx->input_w=ctx->img1.width-ctx->input_start_x;
	if(ctx->input_h>(ctx->img1.height-ctx->input_start_y)) ctx->input_h=ctx->img1.height-ctx->input_start_y;

	if(ctx->req.output_cs_valid) {
		ctx->img2cs = ctx->req.output_cs;

		if(ctx->output_profile&IW_PROFILE_ALWAYSLINEAR) {
			if(ctx->img2cs.cstype!=IW_CSTYPE_LINEAR) {
				iw_warning(ctx,"Forcing output colorspace to linear; required by the output format.");
				iw_make_linear_csdescr(&ctx->img2cs);
			}
		}
	}
	else {
		if(ctx->output_profile&IW_PROFILE_ALWAYSLINEAR) {
			iw_make_linear_csdescr(&ctx->img2cs);
		}
		else {
			iw_make_srgb_csdescr_2(&ctx->img2cs);
		}
	}

	if(ctx->img1.sampletype!=IW_SAMPLETYPE_FLOATINGPOINT) {
		ctx->input_maxcolorcode_int = (1 << ctx->img1.bit_depth)-1;
		ctx->input_maxcolorcode = (double)ctx->input_maxcolorcode_int;

		for(i=0;i<IW_CI_COUNT;i++) {
			if(ctx->img1_ci[i].maxcolorcode_int<=0) {
				ctx->img1_ci[i].maxcolorcode_int = ctx->input_maxcolorcode_int;
			}
			ctx->img1_ci[i].maxcolorcode_dbl = (double)ctx->img1_ci[i].maxcolorcode_int;

			if(ctx->img1_ci[i].maxcolorcode_int != ctx->input_maxcolorcode_int) {
				ctx->support_reduced_input_bitdepths = 1;
			}
		}
	}

	if(ctx->support_reduced_input_bitdepths ||
		ctx->img1.sampletype==IW_SAMPLETYPE_FLOATINGPOINT)
	{
		for(i=0;i<ctx->img1_numchannels_physical;i++) {
			ctx->img1_ci[i].disable_fast_get_sample=1;
		}
	}

	for(i=0;i<2;i++) {  
		for(j=0;j<3;j++) {  
			if(fabs(ctx->resize_settings[i].channel_offset[j])>0.00001) {
				ctx->resize_settings[i].use_offset=1;
			}
		}
	}

	if(ctx->to_grayscale &&
		(ctx->resize_settings[IW_DIMENSION_H].use_offset ||
		ctx->resize_settings[IW_DIMENSION_V].use_offset) )
	{
		iw_warning(ctx,"Disabling channel offset, due to grayscale output.");
		ctx->resize_settings[IW_DIMENSION_H].use_offset=0;
		ctx->resize_settings[IW_DIMENSION_V].use_offset=0;
	}

	decide_how_to_apply_bkgd(ctx);

	for(i=0;i<2;i++) {
		if(ctx->resize_settings[i].use_offset ||
		  (ctx->apply_bkgd &&
		   ctx->apply_bkgd_strategy==IW_BKGD_STRATEGY_EARLY &&
		   ctx->resize_settings[i].edge_policy==IW_EDGE_POLICY_TRANSPARENT))
		{
			ctx->resize_settings[i].disable_rrctx_cache=1;
		}
	}

	decide_strategy(ctx,&strategy1,&strategy2);

	switch(strategy1) {  
	case IW_STRAT1_RGBA_RGBA:
		ctx->intermed_imgtype = IW_IMGTYPE_RGBA;
		break;
	case IW_STRAT1_GA_RGBA:
		ctx->intermed_imgtype = IW_IMGTYPE_RGBA;
		ctx->intermed_ci[0].corresponding_input_channel=0;
		ctx->intermed_ci[1].corresponding_input_channel=0;
		ctx->intermed_ci[2].corresponding_input_channel=0;
		ctx->intermed_ci[3].corresponding_input_channel=1;
		break;
	case IW_STRAT1_RGB_RGB:
	case IW_STRAT1_RGBA_RGB:
		ctx->intermed_imgtype = IW_IMGTYPE_RGB;
		break;
	case IW_STRAT1_G_RGB:
	case IW_STRAT1_GA_RGB:
		ctx->intermed_imgtype = IW_IMGTYPE_RGB;
		ctx->intermed_ci[0].corresponding_input_channel=0;
		ctx->intermed_ci[1].corresponding_input_channel=0;
		ctx->intermed_ci[2].corresponding_input_channel=0;
		break;
	case IW_STRAT1_RGBA_GA:
		ctx->intermed_imgtype = IW_IMGTYPE_GRAYA;
		ctx->intermed_ci[0].cvt_to_grayscale=1;
		ctx->intermed_ci[0].corresponding_input_channel=0;
		ctx->intermed_ci[1].corresponding_input_channel=3;
		break;
	case IW_STRAT1_GA_GA:
		ctx->intermed_imgtype = IW_IMGTYPE_GRAYA;
		break;
	case IW_STRAT1_RGB_G:
		ctx->intermed_imgtype = IW_IMGTYPE_GRAY;
		ctx->intermed_ci[0].cvt_to_grayscale=1;
		ctx->intermed_ci[0].corresponding_input_channel=0;
		break;
	case IW_STRAT1_G_G:
		ctx->intermed_imgtype = IW_IMGTYPE_GRAY;
		ctx->intermed_ci[0].corresponding_input_channel=0;
		break;
	default:
		iw_set_errorf(ctx,"Internal error, unknown strategy %d",strategy1);
		return 0;
	}

	ctx->intermed_numchannels = iw_imgtype_num_channels(ctx->intermed_imgtype);
	ctx->intermed_alpha_channel_index = iw_imgtype_alpha_channel_index(ctx->intermed_imgtype);

	for(i=0;i<ctx->intermed_numchannels;i++) {
		ctx->intermed_ci[i].corresponding_output_channel = i;
	}

	switch(strategy2) {  
	case IW_STRAT2_RGBA_RGBA:
		ctx->img2.imgtype = IW_IMGTYPE_RGBA;
		break;
	case IW_STRAT2_RGB_RGB:
		ctx->img2.imgtype = IW_IMGTYPE_RGB;
		break;
	case IW_STRAT2_RGBA_RGB:
		ctx->img2.imgtype = IW_IMGTYPE_RGB;
		ctx->intermed_ci[3].corresponding_output_channel= -1;
		break;
	case IW_STRAT2_GA_GA:
		ctx->img2.imgtype = IW_IMGTYPE_GRAYA;
		break;
	case IW_STRAT2_G_G:
		ctx->img2.imgtype = IW_IMGTYPE_GRAY;
		break;
	case IW_STRAT2_GA_G:
		ctx->img2.imgtype = IW_IMGTYPE_GRAY;
		ctx->intermed_ci[1].corresponding_output_channel= -1;
		break;
	default:
		iw_set_error(ctx,"Internal error");
		return 0;
	}

	ctx->img2_numchannels = iw_imgtype_num_channels(ctx->img2.imgtype);

	iw_set_intermed_channeltypes(ctx);
	iw_set_out_channeltypes(ctx);

	if(IW_IMGTYPE_HAS_ALPHA(ctx->intermed_imgtype)) {
		for(i=0;i<ctx->intermed_numchannels;i++) {
			if(ctx->intermed_ci[i].channeltype!=IW_CHANNELTYPE_ALPHA)
				ctx->intermed_ci[i].need_unassoc_alpha_processing = 1;
		}
	}


	decide_output_bit_depth(ctx);

	if(ctx->img2.sampletype==IW_SAMPLETYPE_FLOATINGPOINT) {
		flag=0;
		for(i=0;i<IW_NUM_CHANNELTYPES;i++) {
			if(ctx->req.color_count[i]) flag=1;
		}
		if(flag) {
			iw_warning(ctx,"Posterization is not supported with floating point output.");
		}
	}
	else {
		output_maxcolorcode_int = (1 << ctx->img2.bit_depth)-1;

		for(i=0;i<ctx->img2_numchannels;i++) {
			ctx->img2_ci[i].maxcolorcode_int = output_maxcolorcode_int;
		}

		if((ctx->output_profile&IW_PROFILE_REDUCEDBITDEPTHS)) {
			for(i=0;i<ctx->img2_numchannels;i++) {
				int mccr;
				mccr = ctx->req.output_maxcolorcode[ctx->img2_ci[i].channeltype];
				if(mccr>0) {
					if(mccr>output_maxcolorcode_int) mccr=output_maxcolorcode_int;
					ctx->img2_ci[i].maxcolorcode_int = mccr;
				}
			}
		}

		for(i=0;i<ctx->img2_numchannels;i++) {
			if(ctx->img2_ci[i].maxcolorcode_int != output_maxcolorcode_int) {
				ctx->reduced_output_maxcolor_flag = 1;
				ctx->disable_output_lookup_tables = 1;
			}

			ctx->img2_ci[i].maxcolorcode_dbl = (double)ctx->img2_ci[i].maxcolorcode_int;
		}
	}

	for(i=0;i<ctx->img2_numchannels;i++) {
		ctx->img2_ci[i].color_count = ctx->req.color_count[ctx->img2_ci[i].channeltype];
		if(ctx->img2_ci[i].color_count) {
			iw_restrict_to_range(2,ctx->img2_ci[i].maxcolorcode_int,&ctx->img2_ci[i].color_count);
		}
		if(ctx->img2_ci[i].color_count==1+ctx->img2_ci[i].maxcolorcode_int) {
			ctx->img2_ci[i].color_count = 0;
		}

		ctx->img2_ci[i].ditherfamily = ctx->ditherfamily_by_channeltype[ctx->img2_ci[i].channeltype];
		ctx->img2_ci[i].dithersubtype = ctx->dithersubtype_by_channeltype[ctx->img2_ci[i].channeltype];
	}

	for(i=0;i<ctx->img2_numchannels;i++) {
		if(ctx->img2_ci[i].ditherfamily==IW_DITHERFAMILY_ERRDIFF) {
			ctx->uses_errdiffdither=1;
		}
	}

	if(!ctx->support_reduced_input_bitdepths && ctx->img1.sampletype==IW_SAMPLETYPE_UINT) {
		iw_make_x_to_linear_table(ctx,&ctx->input_color_corr_table,&ctx->img1,&ctx->img1cs);
	}

	if(ctx->img1_bkgd_label_set) {
		for(i=0;i<3;i++) {
			ctx->img1_bkgd_label_lin.c[i] = x_to_linear_sample(ctx->img1_bkgd_label_inputcs.c[i],&ctx->img1cs);
		}
		ctx->img1_bkgd_label_lin.c[3] = ctx->img1_bkgd_label_inputcs.c[3];
	}

	if(ctx->apply_bkgd) {
		prepare_apply_bkgd(ctx);
	}

	if(ctx->req.output_rendering_intent==IW_INTENT_UNKNOWN) {
		ctx->img2.rendering_intent = ctx->img1.rendering_intent;
	}
	else {
		ctx->img2.rendering_intent = ctx->req.output_rendering_intent;
	}

	if(ctx->resize_settings[IW_DIMENSION_H].family==IW_RESIZETYPE_AUTO) {
		iw_set_auto_resizetype(ctx,ctx->input_w,ctx->img2.width,IW_DIMENSION_H);
	}
	if(ctx->resize_settings[IW_DIMENSION_V].family==IW_RESIZETYPE_AUTO) {
		iw_set_auto_resizetype(ctx,ctx->input_h,ctx->img2.height,IW_DIMENSION_V);
	}

	if(IW_IMGTYPE_HAS_ALPHA(ctx->img2.imgtype)) {
		if(!ctx->opt_strip_alpha) {
			ctx->opt_palette = 0;
			ctx->opt_binary_trns = 0;
		}
	}

	return 1;
}