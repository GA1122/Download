static int query_formats(AVFilterContext *ctx)
{
    static const enum PixelFormat pix_fmts[] = {
        AV_PIX_FMT_YUV420P,
        AV_PIX_FMT_YUYV422,
        AV_PIX_FMT_ARGB, AV_PIX_FMT_0RGB,
        AV_PIX_FMT_ABGR, AV_PIX_FMT_0BGR,
        AV_PIX_FMT_RGBA, AV_PIX_FMT_RGB0,
        AV_PIX_FMT_BGRA, AV_PIX_FMT_BGR0,
        AV_PIX_FMT_NONE
    };

    ff_set_common_formats(ctx, ff_make_format_list(pix_fmts));

    return 0;
}
