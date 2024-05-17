static vpx_codec_err_t ctrl_set_byte_alignment(vpx_codec_alg_priv_t *ctx,
                                               va_list args) {
 const int legacy_byte_alignment = 0;
 const int min_byte_alignment = 32;
 const int max_byte_alignment = 1024;
 const int byte_alignment = va_arg(args, int);

 if (byte_alignment != legacy_byte_alignment &&
 (byte_alignment < min_byte_alignment ||
       byte_alignment > max_byte_alignment ||
 (byte_alignment & (byte_alignment - 1)) != 0))
 return VPX_CODEC_INVALID_PARAM;

  ctx->byte_alignment = byte_alignment;
 if (ctx->frame_workers) {
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
    frame_worker_data->pbi->common.byte_alignment = byte_alignment;
 }
 return VPX_CODEC_OK;
}
