 vpx_codec_err_t SetFrameBufferFunctions(
 int num_buffers,
 vpx_get_frame_buffer_cb_fn_t cb_get,
 vpx_release_frame_buffer_cb_fn_t cb_release) {
 if (num_buffers > 0) {
      num_buffers_ = num_buffers;
      EXPECT_TRUE(fb_list_.CreateBufferList(num_buffers_));
 }

 return decoder_->SetFrameBufferFunctions(cb_get, cb_release, &fb_list_);
 }
