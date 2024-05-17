static av_cold void uninit(AVFilterContext *ctx)
{
    KerndeintContext *kerndeint = ctx->priv;

    av_free(kerndeint->tmp_data[0]);
}
