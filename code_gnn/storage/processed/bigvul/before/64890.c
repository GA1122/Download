static void rle8_write_unc(struct rle_context *rlectx)
{
	size_t i;
	iw_byte dstbuf[2];

	if(rlectx->unc_len<1) return;
	if(rlectx->unc_len>=3 && (rlectx->unc_len&1)) {
		iw_set_error(rlectx->ctx,"Internal: RLE encode error 4");
		return;
	}
	if(rlectx->unc_len>254) {
		iw_set_error(rlectx->ctx,"Internal: RLE encode error 5");
		return;
	}

	if(rlectx->unc_len<3) {
		for(i=0;i<rlectx->unc_len;i++) {
			dstbuf[0] = 0x01;   
			dstbuf[1] = rlectx->srcrow[i+rlectx->pending_data_start];  
			iwbmp_write(rlectx->wctx,dstbuf,2);
			rlectx->total_bytes_written+=2;
		}
	}
	else {
		dstbuf[0] = 0x00;
		dstbuf[1] = (iw_byte)rlectx->unc_len;
		iwbmp_write(rlectx->wctx,dstbuf,2);
		rlectx->total_bytes_written+=2;
		iwbmp_write(rlectx->wctx,&rlectx->srcrow[rlectx->pending_data_start],rlectx->unc_len);
		rlectx->total_bytes_written+=rlectx->unc_len;
		if(rlectx->unc_len&0x1) {
			dstbuf[0] = 0x00;
			iwbmp_write(rlectx->wctx,dstbuf,1);
			rlectx->total_bytes_written+=1;
		}
	}

	rlectx->pending_data_start+=rlectx->unc_len;
	rlectx->unc_len=0;
}