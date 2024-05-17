void GLES2DecoderImpl::SetResizeCallback(
    Callback1<gfx::Size>::Type* callback) {
  resize_callback_.reset(callback);
}
