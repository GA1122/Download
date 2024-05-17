IW_IMPL(int) iw_file_to_memory(struct iw_context *ctx, struct iw_iodescr *iodescr,
  void **pmem, iw_int64 *psize)
{
	int ret;
	size_t bytesread;

	*pmem=NULL;
	*psize=0;

	if(!iodescr->getfilesize_fn) return 0;

	ret = (*iodescr->getfilesize_fn)(ctx,iodescr,psize);
	if(!ret) return 0;

	*pmem = iw_malloc(ctx,(size_t)*psize);

	ret = (*iodescr->read_fn)(ctx,iodescr,*pmem,(size_t)*psize,&bytesread);
	if(!ret) return 0;
	if((iw_int64)bytesread != *psize) return 0;
	return 1;
}