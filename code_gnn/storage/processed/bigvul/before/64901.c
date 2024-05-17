static void decide_strategy(struct iw_context *ctx, int *ps1, int *ps2)
{
	int s1, s2;

	switch(ctx->img1_imgtype_logical) {
	case IW_IMGTYPE_RGBA:
		if(ctx->to_grayscale) {
			s1=IW_STRAT1_RGBA_GA;
			s2=IW_STRAT2_GA_GA;
		}
		else {
			s1=IW_STRAT1_RGBA_RGBA;
			s2=IW_STRAT2_RGBA_RGBA;
		}
		break;
	case IW_IMGTYPE_RGB:
		if(ctx->to_grayscale) {
			s1=IW_STRAT1_RGB_G;
			s2=IW_STRAT2_G_G;
		}
		else {
			s1=IW_STRAT1_RGB_RGB;
			s2=IW_STRAT2_RGB_RGB;
		}
		break;
	case IW_IMGTYPE_GRAYA:
		if(ctx->to_grayscale) {
			s1=IW_STRAT1_GA_GA;
			s2=IW_STRAT2_GA_GA;
		}
		else {
			s1=IW_STRAT1_GA_RGBA;
			s2=IW_STRAT2_RGBA_RGBA;
		}
		break;
	default:
		if(ctx->to_grayscale) {
			s1=IW_STRAT1_G_G;
			s2=IW_STRAT2_G_G;
		}
		else {
			s1=IW_STRAT1_G_RGB;
			s2=IW_STRAT2_RGB_RGB;
		}
	}

	if(ctx->apply_bkgd && ctx->apply_bkgd_strategy==IW_BKGD_STRATEGY_EARLY) {
		if(s1==IW_STRAT1_RGBA_RGBA) {
			s1=IW_STRAT1_RGBA_RGB;
			s2=IW_STRAT2_RGB_RGB;
		}
		else if(s1==IW_STRAT1_GA_GA) {
			s1=IW_STRAT1_GA_G;
			s2=IW_STRAT2_G_G;
		}
		else if(s1==IW_STRAT1_GA_RGBA) {
			s1=IW_STRAT1_GA_RGB;
			s2=IW_STRAT2_RGB_RGB;
		}
		else if(s1==IW_STRAT1_RGBA_GA) {
			s1=IW_STRAT1_RGBA_G;
			s2=IW_STRAT2_G_G;
		}
	}

	if(ctx->apply_bkgd && !iw_bkgd_has_transparency(ctx)) {
		if(s2==IW_STRAT2_GA_GA) {
			s2=IW_STRAT2_GA_G;
		}
		else if(s2==IW_STRAT2_RGBA_RGBA) {
			s2=IW_STRAT2_RGBA_RGB;
		}
	}

	*ps1 = s1;
	*ps2 = s2;
}