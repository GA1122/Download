void GLES2DecoderImpl::RestoreStateForSimulatedFixedAttribs() {
  api()->glBindBufferFn(GL_ARRAY_BUFFER,
                        state_.bound_array_buffer.get()
                            ? state_.bound_array_buffer->service_id()
                            : 0);
}
