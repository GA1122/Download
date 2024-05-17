error::Error GLES2DecoderImpl::HandleVertexAttribPointer(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::VertexAttribPointer& c =
      *static_cast<const volatile gles2::cmds::VertexAttribPointer*>(cmd_data);
  GLuint indx = c.indx;
  GLint size = c.size;
  GLenum type = c.type;
  GLboolean normalized = static_cast<GLboolean>(c.normalized);
  GLsizei stride = c.stride;
  GLsizei offset = c.offset;

  if (!state_.bound_array_buffer.get() ||
      state_.bound_array_buffer->IsDeleted()) {
    if (offset != 0) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, "glVertexAttribPointer", "offset != 0");
      return error::kNoError;
    }
  }
  const void* ptr = reinterpret_cast<const void*>(offset);
  if (!validators_->vertex_attrib_type.IsValid(type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glVertexAttribPointer", type, "type");
    return error::kNoError;
  }
  if (size < 1 || size > 4) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribPointer", "size GL_INVALID_VALUE");
    return error::kNoError;
  }
  if ((type == GL_INT_2_10_10_10_REV || type == GL_UNSIGNED_INT_2_10_10_10_REV)
      && size != 4) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glVertexAttribPointer", "size != 4");
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
  GLsizei type_size = GLES2Util::GetGLTypeSizeForBuffers(type);
  DCHECK(GLES2Util::IsPOT(type_size));
  if (offset & (type_size - 1)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glVertexAttribPointer", "offset not valid for type");
    return error::kNoError;
  }
  if (stride & (type_size - 1)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glVertexAttribPointer", "stride not valid for type");
    return error::kNoError;
  }

  state_.vertex_attrib_manager->UpdateAttribBaseTypeAndMask(
      indx, SHADER_VARIABLE_FLOAT);

  GLsizei group_size = GLES2Util::GetGroupSizeForBufferType(size, type);
  state_.vertex_attrib_manager
      ->SetAttribInfo(indx,
                      state_.bound_array_buffer.get(),
                      size,
                      type,
                      normalized,
                      stride,
                      stride != 0 ? stride : group_size,
                      offset,
                      GL_FALSE);
  if (type != GL_FIXED || gl_version_info().SupportsFixedType()) {
    api()->glVertexAttribPointerFn(indx, size, type, normalized, stride, ptr);
  }
  return error::kNoError;
}
