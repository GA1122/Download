static void bmpw_convert_row_24(struct iwbmpwcontext *wctx, const iw_byte *srcrow,
	iw_byte *dstrow, int width)
{
	int i;

	if(wctx->img->imgtype==IW_IMGTYPE_GRAY) {
		for(i=0;i<width;i++) {
			dstrow[i*3+0] = srcrow[i];
			dstrow[i*3+1] = srcrow[i];
			dstrow[i*3+2] = srcrow[i];
		}
	}
	else {  
		for(i=0;i<width;i++) {
			dstrow[i*3+0] = srcrow[i*3+2];
			dstrow[i*3+1] = srcrow[i*3+1];
			dstrow[i*3+2] = srcrow[i*3+0];
		}
	}
}