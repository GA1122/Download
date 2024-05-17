static int query_formats(AVFilterContext *ctx)
{
    LutContext *s = ctx->priv;

    const enum AVPixelFormat *pix_fmts = s->is_rgb ? rgb_pix_fmts :
                                       s->is_yuv ? yuv_pix_fmts : all_pix_fmts;

    ff_set_common_formats(ctx, ff_make_format_list(pix_fmts));
    return 0;
}
