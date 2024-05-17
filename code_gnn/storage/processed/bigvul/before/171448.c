static vpx_image_t *decoder_get_frame(vpx_codec_alg_priv_t *ctx,
 vpx_codec_iter_t *iter) {
 vpx_image_t *img = NULL;

 if (ctx->frame_parallel_decode && ctx->available_threads > 0 &&
 !ctx->flushed) {
 return NULL;
 }

 if (ctx->num_cache_frames > 0) {
    release_last_output_frame(ctx);
    ctx->last_show_frame  = ctx->frame_cache[ctx->frame_cache_read].fb_idx;
 if (ctx->need_resync)
 return NULL;
    img = &ctx->frame_cache[ctx->frame_cache_read].img;
    ctx->frame_cache_read = (ctx->frame_cache_read + 1) % FRAME_CACHE_SIZE;
 --ctx->num_cache_frames;
 return img;
 }

 if (*iter == NULL && ctx->frame_workers != NULL) {
 do {
      YV12_BUFFER_CONFIG sd;
 vp9_ppflags_t flags = {0, 0, 0};
 const VPxWorkerInterface *const winterface = vpx_get_worker_interface();
 VPxWorker *const worker =
 &ctx->frame_workers[ctx->next_output_worker_id];
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
      ctx->next_output_worker_id =
 (ctx->next_output_worker_id + 1) % ctx->num_frame_workers;
 if (ctx->base.init_flags & VPX_CODEC_USE_POSTPROC)
        set_ppflags(ctx, &flags);
 if (winterface->sync(worker)) {
 if (frame_worker_data->received_frame == 1) {
 ++ctx->available_threads;
          frame_worker_data->received_frame = 0;
          check_resync(ctx, frame_worker_data->pbi);
 }
 if (vp9_get_raw_frame(frame_worker_data->pbi, &sd, &flags) == 0) {
          VP9_COMMON *const cm = &frame_worker_data->pbi->common;
 RefCntBuffer *const frame_bufs = cm->buffer_pool->frame_bufs;
          release_last_output_frame(ctx);
          ctx->last_show_frame = frame_worker_data->pbi->common.new_fb_idx;
 if (ctx->need_resync)
 return NULL;
          yuvconfig2image(&ctx->img, &sd, frame_worker_data->user_priv);
          ctx->img.fb_priv = frame_bufs[cm->new_fb_idx].raw_frame_buffer.priv;
          img = &ctx->img;
 return img;
 }
 } else {
        frame_worker_data->received_frame = 0;
 ++ctx->available_threads;
        ctx->need_resync = 1;
 if (ctx->flushed != 1)
 return NULL;
 }
 } while (ctx->next_output_worker_id != ctx->next_submit_worker_id);
 }
 return NULL;
}
