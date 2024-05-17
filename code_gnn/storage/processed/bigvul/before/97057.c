static void BumpPixel(GifCtx *ctx)
{
	 
	++(ctx->curx);

	 
	if(ctx->curx == ctx->Width) {
		ctx->curx = 0;

		if(!ctx->Interlace) {
			++(ctx->cury);
		} else {
			switch(ctx->Pass) {

			case 0:
				ctx->cury += 8;
				if(ctx->cury >= ctx->Height) {
					++(ctx->Pass);
					ctx->cury = 4;
				}
				break;

			case 1:
				ctx->cury += 8;
				if(ctx->cury >= ctx->Height) {
					++(ctx->Pass);
					ctx->cury = 2;
				}
				break;

			case 2:
				ctx->cury += 4;
				if(ctx->cury >= ctx->Height) {
					++(ctx->Pass);
					ctx->cury = 1;
				}
				break;

			case 3:
				ctx->cury += 2;
				break;
			}
		}
	}
}
