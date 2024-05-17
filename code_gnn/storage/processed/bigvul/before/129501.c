void GLES2DecoderImpl::SetResizeCallback(
    const base::Callback<void(gfx::Size, float)>& callback) {
  resize_callback_ = callback;
}
