error::Error GLES2DecoderImpl::HandleGetVertexAttribPointerv(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetVertexAttribPointerv& c =
      *static_cast<const volatile gles2::cmds::GetVertexAttribPointerv*>(
          cmd_data);
  GLuint index = static_cast<GLuint>(c.index);
  GLenum pname = static_cast<GLenum>(c.pname);
  typedef cmds::GetVertexAttribPointerv::Result Result;
  Result* result =
      GetSharedMemoryAs<Result*>(c.pointer_shm_id, c.pointer_shm_offset,
                                 Result::ComputeSize(1).ValueOrDie());
  if (!result) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  if (!validators_->vertex_pointer.IsValid(pname)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(
        "glGetVertexAttribPointerv", pname, "pname");
    return error::kNoError;
  }
  if (index >= group_->max_vertex_attribs()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glGetVertexAttribPointerv", "index out of range.");
    return error::kNoError;
  }
  result->SetNumResults(1);
  *result->GetData() =
      state_.vertex_attrib_manager->GetVertexAttrib(index)->offset();
  return error::kNoError;
}
