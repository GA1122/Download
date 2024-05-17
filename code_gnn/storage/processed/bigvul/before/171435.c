static vpx_codec_err_t ctrl_get_last_ref_updates(vpx_codec_alg_priv_t *ctx,
                                                 va_list args) {
 int *const update_info = va_arg(args, int *);

 if (ctx->frame_parallel_decode) {
    set_error_detail(ctx, "Not supported in frame parallel decode");
 return VPX_CODEC_INCAPABLE;
 }

 if (update_info) {
 if (ctx->frame_workers) {
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
 *update_info = frame_worker_data->pbi->refresh_frame_flags;
 return VPX_CODEC_OK;
 } else {
 return VPX_CODEC_ERROR;
 }
 }

 return VPX_CODEC_INVALID_PARAM;
}
