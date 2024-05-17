static vpx_codec_err_t ctrl_get_frame_corrupted(vpx_codec_alg_priv_t *ctx,
                                                va_list args) {
 int *corrupted = va_arg(args, int *);

 if (corrupted) {
 if (ctx->frame_workers) {
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
 RefCntBuffer *const frame_bufs =
          frame_worker_data->pbi->common.buffer_pool->frame_bufs;
 if (frame_worker_data->pbi->common.frame_to_show == NULL)
 return VPX_CODEC_ERROR;
 if (ctx->last_show_frame >= 0)
 *corrupted = frame_bufs[ctx->last_show_frame].buf.corrupted;
 return VPX_CODEC_OK;
 } else {
 return VPX_CODEC_ERROR;
 }
 }

 return VPX_CODEC_INVALID_PARAM;
}
