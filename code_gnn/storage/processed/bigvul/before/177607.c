 static int GetVP9FrameBuffer(void *user_priv, size_t min_size,
 vpx_codec_frame_buffer_t *fb) {
 ExternalFrameBufferMD5Test *const md5Test =
 reinterpret_cast<ExternalFrameBufferMD5Test*>(user_priv);
 return md5Test->fb_list_.GetFreeFrameBuffer(min_size, fb);
 }
