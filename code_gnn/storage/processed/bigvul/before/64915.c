static void init_channel_info(struct iw_context *ctx)
{
	int i;

	ctx->img1_imgtype_logical = ctx->img1.imgtype;

	if(ctx->resize_settings[IW_DIMENSION_H].edge_policy==IW_EDGE_POLICY_TRANSPARENT ||
		ctx->resize_settings[IW_DIMENSION_V].edge_policy==IW_EDGE_POLICY_TRANSPARENT)
	{
		if(ctx->img1.imgtype==IW_IMGTYPE_GRAY) {
			ctx->img1_imgtype_logical = IW_IMGTYPE_GRAYA;
		}
		else if(ctx->img1.imgtype==IW_IMGTYPE_RGB)
			ctx->img1_imgtype_logical = IW_IMGTYPE_RGBA;
	}

	ctx->img1_numchannels_physical = iw_imgtype_num_channels(ctx->img1.imgtype);
	ctx->img1_numchannels_logical = iw_imgtype_num_channels(ctx->img1_imgtype_logical);
	ctx->img1_alpha_channel_index = iw_imgtype_alpha_channel_index(ctx->img1_imgtype_logical);

	iw_set_input_channeltypes(ctx);

	ctx->img2.imgtype = ctx->img1_imgtype_logical;  
	ctx->img2_numchannels = ctx->img1_numchannels_logical;  
	ctx->intermed_numchannels = ctx->img1_numchannels_logical;  

	for(i=0;i<ctx->img1_numchannels_logical;i++) {
		ctx->intermed_ci[i].channeltype = ctx->img1_ci[i].channeltype;
		ctx->intermed_ci[i].corresponding_input_channel = i;
		ctx->img2_ci[i].channeltype = ctx->img1_ci[i].channeltype;
		if(i>=ctx->img1_numchannels_physical) {
			ctx->img1_ci[i].disable_fast_get_sample = 1;
		}
	}
}