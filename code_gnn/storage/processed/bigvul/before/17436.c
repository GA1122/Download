DestroyConstructResourceBytesCtx(ConstructResourceBytesCtx *ctx)
{
    DestroyFragments(&ctx->response);
    ht_destroy(ctx->visitedResources);
}
