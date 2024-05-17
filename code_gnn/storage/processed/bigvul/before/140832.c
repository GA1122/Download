error::Error GLES2DecoderImpl::HandleVertexAttribPointer(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::VertexAttribPointer& c =
      *static_cast<const gles2::cmds::VertexAttribPointer*>(cmd_data);

  if (!state_.bound_array_buffer.get() ||
      state_.bound_array_buffer->IsDeleted()) {
    if (state_.vertex_attrib_manager.get() ==
        state_.default_vertex_attrib_manager.get()) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, "glVertexAttribPointer", "no array buffer bound");
      return error::kNoError;
    } else if (c.offset != 0) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE,
          "glVertexAttribPointer", "client side arrays are not allowed");
      return error::kNoError;
    }
  }

  GLuint indx = c.indx;
  GLint size = c.size;
  GLenum type = c.type;
  GLboolean normalized = static_cast<GLboolean>(c.normalized);
  GLsizei stride = c.stride;
  GLsizei offset = c.offset;
  const void* ptr = reinterpret_cast<const void*>(offset);
  if (!validators_->vertex_attrib_type.IsValid(type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glVertexAttribPointer", type, "type");
    return error::kNoError;
  }
  if (!validators_->vertex_attrib_size.IsValid(size)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribPointer", "size GL_INVALID_VALUE");
    return error::kNoError;
  }
  if (indx >= group_->max_vertex_attribs()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribPointer", "index out of range");
    return error::kNoError;
  }
  if (stride < 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribPointer", "stride < 0");
    return error::kNoError;
  }
  if (stride > 255) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribPointer", "stride > 255");
    return error::kNoError;
  }
  if (offset < 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribPointer", "offset < 0");
    return error::kNoError;
  }
  GLsizei component_size =
      GLES2Util::GetGLTypeSizeForTexturesAndBuffers(type);
  DCHECK(GLES2Util::IsPOT(component_size));
  if (offset & (component_size - 1)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glVertexAttribPointer", "offset not valid for type");
    return error::kNoError;
  }
  if (stride & (component_size - 1)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glVertexAttribPointer", "stride not valid for type");
    return error::kNoError;
  }
  state_.vertex_attrib_manager
      ->SetAttribInfo(indx,
                      state_.bound_array_buffer.get(),
                      size,
                      type,
                      normalized,
                      stride,
                      stride != 0 ? stride : component_size * size,
                      offset);
  if (type != GL_FIXED) {
    glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
  }
  return error::kNoError;
}
