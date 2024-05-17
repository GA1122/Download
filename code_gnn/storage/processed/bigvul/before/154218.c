error::Error GLES2DecoderImpl::HandleDrawElements(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::DrawElements& c =
      *static_cast<const volatile gles2::cmds::DrawElements*>(cmd_data);
  GLsizei count = static_cast<GLsizei>(c.count);
  int32_t offset = static_cast<int32_t>(c.index_offset);
  return DoMultiDrawElements("glDrawElements", false,
                             static_cast<GLenum>(c.mode), &count,
                             static_cast<GLenum>(c.type), &offset, nullptr, 1);
}
