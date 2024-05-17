DestroyConstructClientIdCtx(ConstructClientIdCtx *ctx)
{
    DestroyFragments(&ctx->response);
}
