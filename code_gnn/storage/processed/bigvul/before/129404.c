error::Error GLES2DecoderImpl::HandleDrawElements(
    uint32 immediate_data_size, const cmds::DrawElements& c) {
  return DoDrawElements("glDrawElements",
                        false,
                        static_cast<GLenum>(c.mode),
                        static_cast<GLsizei>(c.count),
                        static_cast<GLenum>(c.type),
                        static_cast<int32>(c.index_offset),
                        0);
}
