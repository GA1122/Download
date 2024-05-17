static vpx_codec_err_t ctrl_set_skip_loop_filter(vpx_codec_alg_priv_t *ctx,
                                                 va_list args) {
  ctx->skip_loop_filter = va_arg(args, int);

 if (ctx->frame_workers) {
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data = (FrameWorkerData *)worker->data1;
    frame_worker_data->pbi->common.skip_loop_filter = ctx->skip_loop_filter;
 }

 return VPX_CODEC_OK;
}
