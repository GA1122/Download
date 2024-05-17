static vpx_codec_err_t decode_one(vpx_codec_alg_priv_t *ctx,
 const uint8_t **data, unsigned int data_sz,
 void *user_priv, int64_t deadline) {
 const VPxWorkerInterface *const winterface = vpx_get_worker_interface();
 (void)deadline;

 if (!ctx->si.h) {
 int is_intra_only = 0;
 const vpx_codec_err_t res =
        decoder_peek_si_internal(*data, data_sz, &ctx->si, &is_intra_only,
                                 ctx->decrypt_cb, ctx->decrypt_state);
 if (res != VPX_CODEC_OK)
 return res;

 if (!ctx->si.is_kf && !is_intra_only)
 return VPX_CODEC_ERROR;
 }

 if (!ctx->frame_parallel_decode) {
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data = (FrameWorkerData *)worker->data1;
    frame_worker_data->data = *data;
    frame_worker_data->data_size = data_sz;
    frame_worker_data->user_priv = user_priv;
    frame_worker_data->received_frame = 1;

    frame_worker_data->pbi->decrypt_cb = ctx->decrypt_cb;
    frame_worker_data->pbi->decrypt_state = ctx->decrypt_state;

    worker->had_error = 0;
    winterface->execute(worker);

 *data = frame_worker_data->data_end;

 if (worker->had_error)
 return update_error_state(ctx, &frame_worker_data->pbi->common.error);

    check_resync(ctx, frame_worker_data->pbi);
 } else {
 VPxWorker *const worker = &ctx->frame_workers[ctx->next_submit_worker_id];
 FrameWorkerData *const frame_worker_data = (FrameWorkerData *)worker->data1;
 if (ctx->next_submit_worker_id != ctx->last_submit_worker_id)
      vp9_frameworker_copy_context(
 &ctx->frame_workers[ctx->next_submit_worker_id],
 &ctx->frame_workers[ctx->last_submit_worker_id]);

    frame_worker_data->pbi->ready_for_new_data = 0;
 if (frame_worker_data->scratch_buffer_size < data_sz) {
      frame_worker_data->scratch_buffer =
 (uint8_t *)vpx_realloc(frame_worker_data->scratch_buffer, data_sz);
 if (frame_worker_data->scratch_buffer == NULL) {
        set_error_detail(ctx, "Failed to reallocate scratch buffer");
 return VPX_CODEC_MEM_ERROR;
 }
      frame_worker_data->scratch_buffer_size = data_sz;
 }
    frame_worker_data->data_size = data_sz;
    memcpy(frame_worker_data->scratch_buffer, *data, data_sz);

    frame_worker_data->frame_decoded = 0;
    frame_worker_data->frame_context_ready = 0;
    frame_worker_data->received_frame = 1;
    frame_worker_data->data = frame_worker_data->scratch_buffer;
    frame_worker_data->user_priv = user_priv;

 if (ctx->next_submit_worker_id != ctx->last_submit_worker_id)
      ctx->last_submit_worker_id =
 (ctx->last_submit_worker_id + 1) % ctx->num_frame_workers;

    ctx->next_submit_worker_id =
 (ctx->next_submit_worker_id + 1) % ctx->num_frame_workers;
 --ctx->available_threads;
    worker->had_error = 0;
    winterface->launch(worker);
 }

 return VPX_CODEC_OK;
}
