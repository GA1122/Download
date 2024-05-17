void GLES2DecoderImpl::RestoreStateForSimulatedFixedAttribs() {
  glBindBuffer(GL_ARRAY_BUFFER,
               bound_array_buffer_ ? bound_array_buffer_->service_id() : 0);
}
