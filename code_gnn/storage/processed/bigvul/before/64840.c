static void bmpr_convert_row_32_16(struct iwbmprcontext *rctx, const iw_byte *src, size_t row)
{
	int i,k;
	unsigned int v,x;
	int numchannels;

	numchannels = rctx->has_alpha_channel ? 4 : 3;

	for(i=0;i<rctx->width;i++) {
		if(rctx->bitcount==32) {
			x = ((unsigned int)src[i*4+0]) | ((unsigned int)src[i*4+1])<<8 |
				((unsigned int)src[i*4+2])<<16 | ((unsigned int)src[i*4+3])<<24;
		}
		else {  
			x = ((unsigned int)src[i*2+0]) | ((unsigned int)src[i*2+1])<<8;
		}
		v = 0;
		for(k=0;k<numchannels;k++) {  
			v = x & rctx->bf_mask[k];
			if(rctx->bf_low_bit[k]>0)
				v >>= rctx->bf_low_bit[k];
			if(rctx->img->bit_depth==16) {
				rctx->img->pixels[row*rctx->img->bpr + i*numchannels*2 + k*2+0] = (iw_byte)(v>>8);
				rctx->img->pixels[row*rctx->img->bpr + i*numchannels*2 + k*2+1] = (iw_byte)(v&0xff);
			}
			else {
				rctx->img->pixels[row*rctx->img->bpr + i*numchannels + k] = (iw_byte)v;
			}
		}
	}
}