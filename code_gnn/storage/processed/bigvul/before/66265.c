static void* emulated_realloc(struct iw_context *ctx, unsigned int flags,
	void *oldmem, size_t oldmem_size, size_t newmem_size)
{
	void *newmem;

	newmem = (*ctx->mallocfn)(ctx->userdata,flags,newmem_size);
	if(oldmem && newmem) {
		if(oldmem_size<newmem_size)
			memcpy(newmem,oldmem,oldmem_size);
		else
			memcpy(newmem,oldmem,newmem_size);
	}
	if(oldmem) {
		(*ctx->freefn)(ctx->userdata,oldmem);
	}
	return newmem;
}