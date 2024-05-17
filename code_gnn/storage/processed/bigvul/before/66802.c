static int iwgif_skip_subblocks(struct iwgifrcontext *rctx)
{
	iw_byte subblock_size;

	while(1) {
		if(!iwgif_read(rctx,rctx->rbuf,1)) return 0;

		subblock_size = rctx->rbuf[0];
		if(subblock_size==0) return 1;

		if(!iwgif_read(rctx,rctx->rbuf,(size_t)subblock_size)) return 0;
	}
}
