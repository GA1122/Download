static vpx_codec_err_t decoder_decode(vpx_codec_alg_priv_t *ctx,
 const uint8_t *data, unsigned int data_sz,
 void *user_priv, long deadline) {
 const uint8_t *data_start = data;
 const uint8_t * const data_end = data + data_sz;
 vpx_codec_err_t res;
 uint32_t frame_sizes[8];
 int frame_count;

 if (data == NULL && data_sz == 0) {
    ctx->flushed = 1;
 return VPX_CODEC_OK;
 }

  ctx->flushed = 0;

 if (ctx->frame_workers == NULL) {
 const vpx_codec_err_t res = init_decoder(ctx);
 if (res != VPX_CODEC_OK)
 return res;
 }

  res = vp9_parse_superframe_index(data, data_sz, frame_sizes, &frame_count,
                                   ctx->decrypt_cb, ctx->decrypt_state);
 if (res != VPX_CODEC_OK)
 return res;

 if (ctx->frame_parallel_decode) {
 if (frame_count > 0) {
 int i;

 for (i = 0; i < frame_count; ++i) {
 const uint8_t *data_start_copy = data_start;
 const uint32_t frame_size = frame_sizes[i];
 if (data_start < data
 || frame_size > (uint32_t) (data_end - data_start)) {
          set_error_detail(ctx, "Invalid frame size in index");
 return VPX_CODEC_CORRUPT_FRAME;
 }

 if (ctx->available_threads == 0) {
 if (ctx->num_cache_frames < FRAME_CACHE_SIZE) {
            wait_worker_and_cache_frame(ctx);
 } else {
            set_error_detail(ctx, "Frame output cache is full.");
 return VPX_CODEC_ERROR;
 }
 }

        res = decode_one(ctx, &data_start_copy, frame_size, user_priv,
                         deadline);
 if (res != VPX_CODEC_OK)
 return res;
        data_start += frame_size;
 }
 } else {
 if (ctx->available_threads == 0) {
 if (ctx->num_cache_frames < FRAME_CACHE_SIZE) {
          wait_worker_and_cache_frame(ctx);
 } else {
          set_error_detail(ctx, "Frame output cache is full.");
 return VPX_CODEC_ERROR;
 }
 }

      res = decode_one(ctx, &data, data_sz, user_priv, deadline);
 if (res != VPX_CODEC_OK)
 return res;
 }
 } else {
 if (frame_count > 0) {
 int i;

 for (i = 0; i < frame_count; ++i) {
 const uint8_t *data_start_copy = data_start;
 const uint32_t frame_size = frame_sizes[i];
 vpx_codec_err_t res;
 if (data_start < data
 || frame_size > (uint32_t) (data_end - data_start)) {
          set_error_detail(ctx, "Invalid frame size in index");
 return VPX_CODEC_CORRUPT_FRAME;
 }

        res = decode_one(ctx, &data_start_copy, frame_size, user_priv,
                         deadline);
 if (res != VPX_CODEC_OK)
 return res;

        data_start += frame_size;
 }
 } else {
 while (data_start < data_end) {
 const uint32_t frame_size = (uint32_t) (data_end - data_start);
 const vpx_codec_err_t res = decode_one(ctx, &data_start, frame_size,
                                               user_priv, deadline);
 if (res != VPX_CODEC_OK)
 return res;

 while (data_start < data_end) {
 const uint8_t marker = read_marker(ctx->decrypt_cb,
                                             ctx->decrypt_state, data_start);
 if (marker)
 break;
 ++data_start;
 }
 }
 }
 }

 return res;
}
