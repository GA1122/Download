static int get_output_bkgd_label_lin(struct iw_context *ctx, struct iw_color *clr)
{
	clr->c[0] = 1.0; clr->c[1] = 0.0; clr->c[2] = 1.0; clr->c[3] = 1.0;

	if(ctx->req.suppress_output_bkgd_label) return 0;

	if(ctx->req.output_bkgd_label_valid) {
		*clr = ctx->req.output_bkgd_label;
		return 1;
	}

	if(ctx->img1_bkgd_label_set) {
		*clr = ctx->img1_bkgd_label_lin;
		return 1;
	}

	return 0;
}
