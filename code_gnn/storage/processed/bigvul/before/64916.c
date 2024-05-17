static iw_tmpsample iw_color_to_grayscale(struct iw_context *ctx,
	iw_tmpsample r, iw_tmpsample g, iw_tmpsample b)
{
	iw_tmpsample v0,v1,v2;

	switch(ctx->grayscale_formula) {
	case IW_GSF_WEIGHTED:
		return ctx->grayscale_weight[0]*r +
			ctx->grayscale_weight[1]*g +
			ctx->grayscale_weight[2]*b;
	case IW_GSF_ORDERBYVALUE:
		if(g<=r) { v0=r; v1=g; }
		else { v0=g; v1=r; }
		if(b<=v1) {
			v2=b;
		}
		else {
			v2=v1;
			if(b<=v0) { v1=b; }
			else { v1=v0; v0=b; }
		}
		return ctx->grayscale_weight[0]*v0 +
			ctx->grayscale_weight[1]*v1 +
			ctx->grayscale_weight[2]*v2;
	}
	return 0.0;
}