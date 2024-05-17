int get_vp9_zero_frame_buffer(void *user_priv, size_t min_size,
 vpx_codec_frame_buffer_t *fb) {
 ExternalFrameBufferList *const fb_list =
 reinterpret_cast<ExternalFrameBufferList*>(user_priv);
 return fb_list->GetZeroFrameBuffer(min_size, fb);
}
