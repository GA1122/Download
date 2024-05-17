static void decide_output_bit_depth(struct iw_context *ctx)
{
	if(ctx->output_profile&IW_PROFILE_HDRI) {
		ctx->img2.sampletype=IW_SAMPLETYPE_FLOATINGPOINT;
	}
	else {
		ctx->img2.sampletype=IW_SAMPLETYPE_UINT;
	}

	if(ctx->img2.sampletype==IW_SAMPLETYPE_FLOATINGPOINT) {
		ctx->img2.bit_depth=32;
		return;
	}


	if(ctx->req.output_depth>8 && (ctx->output_profile&IW_PROFILE_16BPS)) {
		ctx->img2.bit_depth=16;
	}
	else {
		if(ctx->req.output_depth>8) {
			iw_warning(ctx,"Reducing depth to 8; required by the output format.");
		}
		ctx->img2.bit_depth=8;
	}
}
