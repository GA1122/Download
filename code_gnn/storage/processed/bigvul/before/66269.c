IW_IMPL(void) iw_free(struct iw_context *ctx, void *mem)
{
	if(!mem) return;
	(*ctx->freefn)(ctx->userdata,mem);
}
