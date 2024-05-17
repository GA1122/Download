static AVFrame *get_video_buffer(AVFilterLink *inlink, int w, int h)
{
    AVFilterContext   *ctx        = inlink->dst;
    AVFilterLink      *outlink    = ctx->outputs[0];

    return ff_get_video_buffer(outlink, w, h);
}
