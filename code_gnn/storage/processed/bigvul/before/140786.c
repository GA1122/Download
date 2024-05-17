error::Error GLES2DecoderImpl::HandleDrawArrays(uint32 immediate_data_size,
                                                const void* cmd_data) {
  const cmds::DrawArrays& c = *static_cast<const cmds::DrawArrays*>(cmd_data);
  return DoDrawArrays("glDrawArrays",
                      false,
                      static_cast<GLenum>(c.mode),
                      static_cast<GLint>(c.first),
                      static_cast<GLsizei>(c.count),
                      1);
}
