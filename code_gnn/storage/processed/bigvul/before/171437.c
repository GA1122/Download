static vpx_codec_err_t ctrl_get_render_size(vpx_codec_alg_priv_t *ctx,
                                            va_list args) {
 int *const render_size = va_arg(args, int *);

 if (ctx->frame_parallel_decode) {
    set_error_detail(ctx, "Not supported in frame parallel decode");
 return VPX_CODEC_INCAPABLE;
 }

 if (render_size) {
 if (ctx->frame_workers) {
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
 const VP9_COMMON *const cm = &frame_worker_data->pbi->common;
      render_size[0] = cm->render_width;
      render_size[1] = cm->render_height;
 return VPX_CODEC_OK;
 } else {
 return VPX_CODEC_ERROR;
 }
 }

 return VPX_CODEC_INVALID_PARAM;
}
