void GLES2DecoderImpl::DoCopyBufferSubData(GLenum readtarget,
                                           GLenum writetarget,
                                           GLintptr readoffset,
                                           GLintptr writeoffset,
                                           GLsizeiptr size) {
  buffer_manager()->ValidateAndDoCopyBufferSubData(
      &state_, readtarget, writetarget, readoffset, writeoffset, size);
}
