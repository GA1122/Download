GLuint GLES2DecoderImpl::GetBackbufferServiceId() {
  return (offscreen_target_frame_buffer_.get()) ?
      offscreen_target_frame_buffer_->id() :
      surface_->GetBackingFrameBufferObject();
}
