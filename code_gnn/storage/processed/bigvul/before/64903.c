static double get_final_sample_using_nc_tbl(struct iw_context *ctx, iw_tmpsample samp_lin)
{
	unsigned int x;
	unsigned int d;



	x = 127;
	d = 64;

	while(1) {
		if(x>254 || ctx->nearest_color_table[x] > samp_lin)
			x -= d;
		else
			x += d;

		if(d==1) {
			if(x>254 || ctx->nearest_color_table[x] > samp_lin)
				return (double)(x);
			else
				return (double)(x+1);
		}

		d = d/2;
	}
}
