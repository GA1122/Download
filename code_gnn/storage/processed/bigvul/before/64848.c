static void bmpw_convert_row_16_32(struct iwbmpwcontext *wctx, const iw_byte *srcrow,
	iw_byte *dstrow, int width)
{
	int i,k;
	unsigned int v;
	int num_src_samples;
	unsigned int src_sample[4];

	for(k=0;k<4;k++) src_sample[k]=0;

	num_src_samples = iw_imgtype_num_channels(wctx->img->imgtype);

	for(i=0;i<width;i++) {

		for(k=0;k<num_src_samples;k++) {
			if(wctx->img->bit_depth==16) {
				src_sample[k] = (srcrow[num_src_samples*2*i + k*2]<<8) | srcrow[num_src_samples*2*i + k*2 +1];
			}
			else {
				src_sample[k] = srcrow[num_src_samples*i + k];
			}
		}

		switch(wctx->img->imgtype) {
		case IW_IMGTYPE_GRAY:
			v = src_sample[0] << wctx->bf_amt_to_shift[0];
			v |= src_sample[0] << wctx->bf_amt_to_shift[1];
			v |= src_sample[0] << wctx->bf_amt_to_shift[2];
			break;
		case IW_IMGTYPE_RGBA:
			v = src_sample[0] << wctx->bf_amt_to_shift[0];
			v |= src_sample[1] << wctx->bf_amt_to_shift[1];
			v |= src_sample[2] << wctx->bf_amt_to_shift[2];
			v |= src_sample[3] << wctx->bf_amt_to_shift[3];
			break;
		case IW_IMGTYPE_GRAYA:
			v = src_sample[0] << wctx->bf_amt_to_shift[0];
			v |= src_sample[0] << wctx->bf_amt_to_shift[1];
			v |= src_sample[0] << wctx->bf_amt_to_shift[2];
			v |= src_sample[1] << wctx->bf_amt_to_shift[3];
			break;
		default:
			v = src_sample[0] << wctx->bf_amt_to_shift[0];
			v |= src_sample[1] << wctx->bf_amt_to_shift[1];
			v |= src_sample[2] << wctx->bf_amt_to_shift[2];
		}

		if(wctx->bitcount==32) {
			dstrow[i*4+0] = (iw_byte)(v&0xff);
			dstrow[i*4+1] = (iw_byte)((v&0x0000ff00)>>8);
			dstrow[i*4+2] = (iw_byte)((v&0x00ff0000)>>16);
			dstrow[i*4+3] = (iw_byte)((v&0xff000000)>>24);
		}
		else {
			dstrow[i*2+0] = (iw_byte)(v&0xff);
			dstrow[i*2+1] = (iw_byte)(v>>8);
		}
	}
}