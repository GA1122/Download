static IW_INLINE void translate_coords(struct iw_context *ctx,
	int x, int y, int *prx, int *pry)
{
	if(ctx->img1.orient_transform==0) {
		*prx = ctx->input_start_x+x;
		*pry = ctx->input_start_y+y;
		return;
	}

	switch(ctx->img1.orient_transform) {
	case 1:  
		*prx = ctx->img1.width - 1 - (ctx->input_start_x+x);
		*pry = ctx->input_start_y+y;
		break;
	case 2:  
		*prx = ctx->input_start_x+x;
		*pry = ctx->img1.height - 1 - (ctx->input_start_y+y);
		break;
	case 3:  
		*prx = ctx->img1.width - 1 - (ctx->input_start_x+x);
		*pry = ctx->img1.height - 1 - (ctx->input_start_y+y);
		break;
	case 4:
		*prx = ctx->input_start_y+y;
		*pry = ctx->input_start_x+x;
		break;
	case 5:
		*prx = ctx->input_start_y+y;
		*pry = ctx->img1.width - 1 - (ctx->input_start_x+x);
		break;
	case 6:
		*prx = ctx->img1.height - 1 - (ctx->input_start_y+y);
		*pry = ctx->input_start_x+x;
		break;
	case 7:
		*prx = ctx->img1.height - 1 - (ctx->input_start_y+y);
		*pry = ctx->img1.width - 1 - (ctx->input_start_x+x);
		break;
	default:
		*prx = 0;
		*pry = 0;
		break;
	}
}