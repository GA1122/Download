static void rle4_write_unc(struct rle_context *rlectx)
{
	iw_byte dstbuf[128];
	size_t pixels_to_write;
	size_t bytes_to_write;

	if(rlectx->unc_len<1) return;


	while(rlectx->unc_len>0) {
		pixels_to_write = rle4_get_best_unc_split(rlectx->unc_len);

		if(pixels_to_write<3) {
			dstbuf[0] = (iw_byte)pixels_to_write;
			dstbuf[1] = (rlectx->srcrow[rlectx->pending_data_start]<<4);
			if(pixels_to_write>1)
				dstbuf[1] |= (rlectx->srcrow[rlectx->pending_data_start+1]);

			bytes_to_write = 2;
		}
		else {
			size_t i;

			dstbuf[0] = 0x00;
			dstbuf[1] = (iw_byte)pixels_to_write;
			iwbmp_write(rlectx->wctx,dstbuf,2);
			rlectx->total_bytes_written+=2;

			bytes_to_write = 2*((pixels_to_write+3)/4);
			iw_zeromem(dstbuf,bytes_to_write);

			for(i=0;i<pixels_to_write;i++) {
				if(i&0x1) dstbuf[i/2] |= rlectx->srcrow[rlectx->pending_data_start+i];
				else dstbuf[i/2] = rlectx->srcrow[rlectx->pending_data_start+i]<<4;
			}
		}

		iwbmp_write(rlectx->wctx,dstbuf,bytes_to_write);
		rlectx->total_bytes_written += bytes_to_write;
		rlectx->unc_len -= pixels_to_write;
		rlectx->pending_data_start += pixels_to_write;
	}
}
