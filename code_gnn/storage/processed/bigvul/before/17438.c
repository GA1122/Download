InitConstructClientIdCtx(ConstructClientIdCtx *ctx)
{
    ctx->numIds = 0;
    ctx->resultBytes = 0;
    xorg_list_init(&ctx->response);
    memset(ctx->sentClientMasks, 0, sizeof(ctx->sentClientMasks));
}
