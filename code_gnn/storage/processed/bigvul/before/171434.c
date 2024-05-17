static vpx_codec_err_t ctrl_get_frame_size(vpx_codec_alg_priv_t *ctx,
                                           va_list args) {
 int *const frame_size = va_arg(args, int *);

 if (ctx->frame_parallel_decode) {
    set_error_detail(ctx, "Not supported in frame parallel decode");
 return VPX_CODEC_INCAPABLE;
 }

 if (frame_size) {
 if (ctx->frame_workers) {
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
 const VP9_COMMON *const cm = &frame_worker_data->pbi->common;
      frame_size[0] = cm->width;
      frame_size[1] = cm->height;
 return VPX_CODEC_OK;
 } else {
 return VPX_CODEC_ERROR;
 }
 }

 return VPX_CODEC_INVALID_PARAM;
}
