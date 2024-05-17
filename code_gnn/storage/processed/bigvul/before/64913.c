static unsigned int get_raw_sample_int(struct iw_context *ctx,
	   int x, int y, int channel)
{
	int rx,ry;  

	translate_coords(ctx,x,y,&rx,&ry);

	switch(ctx->img1.bit_depth) {
	case 8: return get_raw_sample_8(ctx,rx,ry,channel);
	case 1: return get_raw_sample_1(ctx,rx,ry);
	case 16: return get_raw_sample_16(ctx,rx,ry,channel);
	case 4: return get_raw_sample_4(ctx,rx,ry);
	case 2: return get_raw_sample_2(ctx,rx,ry);
	}
	return 0;
}