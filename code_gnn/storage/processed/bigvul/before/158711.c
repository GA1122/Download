error::Error GLES2DecoderImpl::HandleDrawArrays(uint32_t immediate_data_size,
                                                const volatile void* cmd_data) {
  const volatile cmds::DrawArrays& c =
      *static_cast<const volatile cmds::DrawArrays*>(cmd_data);
  return DoDrawArrays("glDrawArrays",
                      false,
                      static_cast<GLenum>(c.mode),
                      static_cast<GLint>(c.first),
                      static_cast<GLsizei>(c.count),
                      1);
}
