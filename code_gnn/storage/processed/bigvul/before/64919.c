static void iw_errdiff_dither(struct iw_context *ctx,int dithersubtype,
	double err,int x,int y)
{
	int fwd;
	const double *m;


	static const double matrix_list[][12] = {
	{                          7.0/16, 0.0,      
	   0.0   , 3.0/16, 5.0/16, 1.0/16, 0.0,
	   0.0   ,    0.0,    0.0, 0.0   , 0.0    },
	{                          7.0/48, 5.0/48,   
	   3.0/48, 5.0/48, 7.0/48, 5.0/48, 3.0/48,
	   1.0/48, 3.0/48, 5.0/48, 3.0/48, 1.0/48 },
	{                          8.0/42, 4.0/42,   
	   2.0/42, 4.0/42, 8.0/42, 4.0/42, 2.0/42,
	   1.0/42, 2.0/42, 4.0/42, 2.0/42, 1.0/42 },
	{                          8.0/32, 4.0/32,   
	   2.0/32, 4.0/32, 8.0/32, 4.0/32, 2.0/32,
	   0.0   , 0.0   , 0.0   , 0.0   , 0.0    },
	{                          5.0/32, 3.0/32,   
	   2.0/32, 4.0/32, 5.0/32, 4.0/32, 2.0/32,
	      0.0, 2.0/32, 3.0/32, 2.0/32, 0.0    },
	{                          4.0/16, 3.0/16,   
	   1.0/16, 2.0/16, 3.0/16, 2.0/16, 1.0/16,
	   0.0   , 0.0   , 0.0   , 0.0   , 0.0    },
	{                          2.0/4 , 0.0,      
	   0.0   , 1.0/4 , 1.0/4 , 0.0   , 0.0,
	   0.0   , 0.0   , 0.0   , 0.0   , 0.0    },
	{                          1.0/8 , 1.0/8,    
	   0.0   , 1.0/8 , 1.0/8 , 1.0/8 , 0.0,
	   0.0   , 0.0   , 1.0/8 , 0.0   , 0.0    }
	};

	if(dithersubtype<=7)
		m = matrix_list[dithersubtype];
	else
		m = matrix_list[0];

	fwd = (y%2)?(-1):1;

	if((x-fwd)>=0 && (x-fwd)<ctx->img2.width) {
		if((x-2*fwd)>=0 && (x-2*fwd)<ctx->img2.width) {
			ctx->dither_errors[1][x-2*fwd] += err*(m[2]);
			ctx->dither_errors[2][x-2*fwd] += err*(m[7]);
		}
		ctx->dither_errors[1][x-fwd] += err*(m[3]);
		ctx->dither_errors[2][x-fwd] += err*(m[8]);
	}

	ctx->dither_errors[1][x] += err*(m[4]);
	ctx->dither_errors[2][x] += err*(m[9]);

	if((x+fwd)>=0 && (x+fwd)<ctx->img2.width) {
		ctx->dither_errors[0][x+fwd] += err*(m[0]);
		ctx->dither_errors[1][x+fwd] += err*(m[5]);
		ctx->dither_errors[2][x+fwd] += err*(m[10]);
		if((x+2*fwd)>=0 && (x+2*fwd)<ctx->img2.width) {
			ctx->dither_errors[0][x+2*fwd] += err*(m[1]);
			ctx->dither_errors[1][x+2*fwd] += err*(m[6]);
			ctx->dither_errors[2][x+2*fwd] += err*(m[11]);
		}
	}
}