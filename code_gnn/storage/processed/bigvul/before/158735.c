error::Error GLES2DecoderImpl::HandleVertexAttribIPointer(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::VertexAttribIPointer& c =
      *static_cast<const volatile gles2::cmds::VertexAttribIPointer*>(cmd_data);
  GLuint indx = c.indx;
  GLint size = c.size;
  GLenum type = c.type;
  GLsizei stride = c.stride;
  GLsizei offset = c.offset;

  if (!state_.bound_array_buffer.get() ||
      state_.bound_array_buffer->IsDeleted()) {
    if (offset != 0) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, "glVertexAttribIPointer", "offset != 0");
      return error::kNoError;
    }
  }

  const void* ptr = reinterpret_cast<const void*>(offset);
  if (!validators_->vertex_attrib_i_type.IsValid(type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glVertexAttribIPointer", type, "type");
    return error::kNoError;
  }
  if (size < 1 || size > 4) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribIPointer", "size GL_INVALID_VALUE");
    return error::kNoError;
  }
  if (indx >= group_->max_vertex_attribs()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribIPointer", "index out of range");
    return error::kNoError;
  }
  if (stride < 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribIPointer", "stride < 0");
    return error::kNoError;
  }
  if (stride > 255) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribIPointer", "stride > 255");
    return error::kNoError;
  }
  if (offset < 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glVertexAttribIPointer", "offset < 0");
    return error::kNoError;
  }
  GLsizei type_size = GLES2Util::GetGLTypeSizeForBuffers(type);
  DCHECK(GLES2Util::IsPOT(type_size));
  if (offset & (type_size - 1)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glVertexAttribIPointer", "offset not valid for type");
    return error::kNoError;
  }
  if (stride & (type_size - 1)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glVertexAttribIPointer", "stride not valid for type");
    return error::kNoError;
  }

  GLenum base_type = (type == GL_BYTE || type == GL_SHORT || type == GL_INT) ?
                      SHADER_VARIABLE_INT : SHADER_VARIABLE_UINT;
  state_.vertex_attrib_manager->UpdateAttribBaseTypeAndMask(indx, base_type);

  GLsizei group_size = GLES2Util::GetGroupSizeForBufferType(size, type);
  state_.vertex_attrib_manager
      ->SetAttribInfo(indx,
                      state_.bound_array_buffer.get(),
                      size,
                      type,
                      GL_FALSE,
                      stride,
                      stride != 0 ? stride : group_size,
                      offset,
                      GL_TRUE);
  api()->glVertexAttribIPointerFn(indx, size, type, stride, ptr);
  return error::kNoError;
}
