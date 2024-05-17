static void init_buffer_callbacks(vpx_codec_alg_priv_t *ctx) {
 int i;

 for (i = 0; i < ctx->num_frame_workers; ++i) {
 VPxWorker *const worker = &ctx->frame_workers[i];
 FrameWorkerData *const frame_worker_data = (FrameWorkerData *)worker->data1;
    VP9_COMMON *const cm = &frame_worker_data->pbi->common;
 BufferPool *const pool = cm->buffer_pool;

    cm->new_fb_idx = INVALID_IDX;
    cm->byte_alignment = ctx->byte_alignment;
    cm->skip_loop_filter = ctx->skip_loop_filter;

 if (ctx->get_ext_fb_cb != NULL && ctx->release_ext_fb_cb != NULL) {
      pool->get_fb_cb = ctx->get_ext_fb_cb;
      pool->release_fb_cb = ctx->release_ext_fb_cb;
      pool->cb_priv = ctx->ext_priv;
 } else {
      pool->get_fb_cb = vp9_get_frame_buffer;
      pool->release_fb_cb = vp9_release_frame_buffer;

 if (vp9_alloc_internal_frame_buffers(&pool->int_frame_buffers))
        vpx_internal_error(&cm->error, VPX_CODEC_MEM_ERROR,
 "Failed to initialize internal frame buffers");

      pool->cb_priv = &pool->int_frame_buffers;
 }
 }
}
