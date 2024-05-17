static int iwbmp_read_bitfields(struct iwbmprcontext *rctx)
{
	iw_byte buf[12];
	int k;

	if(!iwbmp_read(rctx,buf,12)) return 0;

	for(k=0;k<3;k++) {
		rctx->bf_mask[k] = iw_get_ui32le(&buf[k*4]);
		if(rctx->bf_mask[k]==0) return 0;

		if(!process_bf_mask(rctx,k)) return 0;
	}

	return 1;
}