void GLES2DecoderImpl::SetResizeCallback(
    const base::Callback<void(gfx::Size)>& callback) {
  resize_callback_ = callback;
}
