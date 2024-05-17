static void wait_worker_and_cache_frame(vpx_codec_alg_priv_t *ctx) {
  YV12_BUFFER_CONFIG sd;
 vp9_ppflags_t flags = {0, 0, 0};
 const VPxWorkerInterface *const winterface = vpx_get_worker_interface();
 VPxWorker *const worker = &ctx->frame_workers[ctx->next_output_worker_id];
 FrameWorkerData *const frame_worker_data = (FrameWorkerData *)worker->data1;
  ctx->next_output_worker_id =
 (ctx->next_output_worker_id + 1) % ctx->num_frame_workers;
  winterface->sync(worker);
  frame_worker_data->received_frame = 0;
 ++ctx->available_threads;

  check_resync(ctx, frame_worker_data->pbi);

 if (vp9_get_raw_frame(frame_worker_data->pbi, &sd, &flags) == 0) {
    VP9_COMMON *const cm = &frame_worker_data->pbi->common;
 RefCntBuffer *const frame_bufs = cm->buffer_pool->frame_bufs;
    ctx->frame_cache[ctx->frame_cache_write].fb_idx = cm->new_fb_idx;
    yuvconfig2image(&ctx->frame_cache[ctx->frame_cache_write].img, &sd,
                    frame_worker_data->user_priv);
    ctx->frame_cache[ctx->frame_cache_write].img.fb_priv =
        frame_bufs[cm->new_fb_idx].raw_frame_buffer.priv;
    ctx->frame_cache_write =
 (ctx->frame_cache_write + 1) % FRAME_CACHE_SIZE;
 ++ctx->num_cache_frames;
 }
}
