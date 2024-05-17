static void negate_target_image(struct iw_context *ctx)
{
	int channel;
	struct iw_channelinfo_out *ci;
	int i,j;
	size_t pos;
	iw_float32 s;
	unsigned int n;

	for(channel=0; channel<ctx->img2_numchannels; channel++) {
		ci = &ctx->img2_ci[channel];
		if(ci->channeltype == IW_CHANNELTYPE_ALPHA) continue;  

		if(ctx->img2.sampletype==IW_SAMPLETYPE_FLOATINGPOINT) {
			for(j=0; j<ctx->img2.height; j++) {
				for(i=0; i<ctx->img2.width; i++) {
					pos = j*ctx->img2.bpr + ctx->img2_numchannels*i*4 + channel*4;
					s = iw_get_float32(&ctx->img2.pixels[pos]);
					iw_put_float32(&ctx->img2.pixels[pos], ((iw_float32)1.0)-s);
				}
			}
		}
		else if(ctx->img2.bit_depth==8) {
			for(j=0; j<ctx->img2.height; j++) {
				for(i=0; i<ctx->img2.width; i++) {
					pos = j*ctx->img2.bpr + ctx->img2_numchannels*i + channel;
					ctx->img2.pixels[pos] = ci->maxcolorcode_int-ctx->img2.pixels[pos];
				}
			}
		}
		else if(ctx->img2.bit_depth==16) {
			for(j=0; j<ctx->img2.height; j++) {
				for(i=0; i<ctx->img2.width; i++) {
					pos = j*ctx->img2.bpr + ctx->img2_numchannels*i*2 + channel*2;
					n = ctx->img2.pixels[pos]*256 + ctx->img2.pixels[pos+1];
					n = ci->maxcolorcode_int - n;
					ctx->img2.pixels[pos] = (n&0xff00)>>8;
					ctx->img2.pixels[pos+1] = n&0x00ff;
				}
			}
		}
	}
}