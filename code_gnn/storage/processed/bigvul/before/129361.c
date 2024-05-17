GLuint GLES2DecoderImpl::GetBackbufferServiceId() const {
  return (offscreen_target_frame_buffer_.get())
             ? offscreen_target_frame_buffer_->id()
             : (surface_.get() ? surface_->GetBackingFrameBufferObject() : 0);
}
