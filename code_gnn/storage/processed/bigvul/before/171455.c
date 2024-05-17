static vpx_codec_err_t init_decoder(vpx_codec_alg_priv_t *ctx) {
 int i;
 const VPxWorkerInterface *const winterface = vpx_get_worker_interface();

  ctx->last_show_frame = -1;
  ctx->next_submit_worker_id = 0;
  ctx->last_submit_worker_id = 0;
  ctx->next_output_worker_id = 0;
  ctx->frame_cache_read = 0;
  ctx->frame_cache_write = 0;
  ctx->num_cache_frames = 0;
  ctx->need_resync = 1;
  ctx->num_frame_workers =
 (ctx->frame_parallel_decode == 1) ? ctx->cfg.threads: 1;
 if (ctx->num_frame_workers > MAX_DECODE_THREADS)
    ctx->num_frame_workers = MAX_DECODE_THREADS;
  ctx->available_threads = ctx->num_frame_workers;
  ctx->flushed = 0;

  ctx->buffer_pool = (BufferPool *)vpx_calloc(1, sizeof(BufferPool));
 if (ctx->buffer_pool == NULL)
 return VPX_CODEC_MEM_ERROR;

#if CONFIG_MULTITHREAD
 if (pthread_mutex_init(&ctx->buffer_pool->pool_mutex, NULL)) {
      set_error_detail(ctx, "Failed to allocate buffer pool mutex");
 return VPX_CODEC_MEM_ERROR;
 }
#endif

  ctx->frame_workers = (VPxWorker *)
      vpx_malloc(ctx->num_frame_workers * sizeof(*ctx->frame_workers));
 if (ctx->frame_workers == NULL) {
    set_error_detail(ctx, "Failed to allocate frame_workers");
 return VPX_CODEC_MEM_ERROR;
 }

 for (i = 0; i < ctx->num_frame_workers; ++i) {
 VPxWorker *const worker = &ctx->frame_workers[i];
 FrameWorkerData *frame_worker_data = NULL;
    winterface->init(worker);
    worker->data1 = vpx_memalign(32, sizeof(FrameWorkerData));
 if (worker->data1 == NULL) {
      set_error_detail(ctx, "Failed to allocate frame_worker_data");
 return VPX_CODEC_MEM_ERROR;
 }
    frame_worker_data = (FrameWorkerData *)worker->data1;
    frame_worker_data->pbi = vp9_decoder_create(ctx->buffer_pool);
 if (frame_worker_data->pbi == NULL) {
      set_error_detail(ctx, "Failed to allocate frame_worker_data");
 return VPX_CODEC_MEM_ERROR;
 }
    frame_worker_data->pbi->frame_worker_owner = worker;
    frame_worker_data->worker_id = i;
    frame_worker_data->scratch_buffer = NULL;
    frame_worker_data->scratch_buffer_size = 0;
    frame_worker_data->frame_context_ready = 0;
    frame_worker_data->received_frame = 0;
#if CONFIG_MULTITHREAD
 if (pthread_mutex_init(&frame_worker_data->stats_mutex, NULL)) {
      set_error_detail(ctx, "Failed to allocate frame_worker_data mutex");
 return VPX_CODEC_MEM_ERROR;
 }

 if (pthread_cond_init(&frame_worker_data->stats_cond, NULL)) {
      set_error_detail(ctx, "Failed to allocate frame_worker_data cond");
 return VPX_CODEC_MEM_ERROR;
 }
#endif
    frame_worker_data->pbi->max_threads =
 (ctx->frame_parallel_decode == 0) ? ctx->cfg.threads : 0;

    frame_worker_data->pbi->inv_tile_order = ctx->invert_tile_order;
    frame_worker_data->pbi->frame_parallel_decode = ctx->frame_parallel_decode;
    frame_worker_data->pbi->common.frame_parallel_decode =
        ctx->frame_parallel_decode;
    worker->hook = (VPxWorkerHook)frame_worker_hook;
 if (!winterface->reset(worker)) {
      set_error_detail(ctx, "Frame Worker thread creation failed");
 return VPX_CODEC_MEM_ERROR;
 }
 }

 if (!ctx->postproc_cfg_set &&
 (ctx->base.init_flags & VPX_CODEC_USE_POSTPROC))
    set_default_ppflags(&ctx->postproc_cfg);

  init_buffer_callbacks(ctx);

 return VPX_CODEC_OK;
}
