error::Error GLES2DecoderImpl::HandleVertexAttribIPointer(
    uint32 immediate_data_size,
    const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::VertexAttribIPointer& c =
      *static_cast<const gles2::cmds::VertexAttribIPointer*>(cmd_data);
  GLuint indx = c.indx;
  GLint size = c.size;
  GLenum type = c.type;
  GLsizei stride = c.stride;
  GLsizei offset = c.offset;
  const void* ptr = reinterpret_cast<const void*>(offset);
  glVertexAttribIPointer(indx, size, type, stride, ptr);
  return error::kNoError;
}
