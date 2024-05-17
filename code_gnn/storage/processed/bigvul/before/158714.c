error::Error GLES2DecoderImpl::HandleGetActiveUniformBlockiv(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::GetActiveUniformBlockiv& c =
      *static_cast<const volatile gles2::cmds::GetActiveUniformBlockiv*>(
          cmd_data);
  GLuint program_id = c.program;
  GLuint index = static_cast<GLuint>(c.index);
  GLenum pname = static_cast<GLenum>(c.pname);
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetActiveUniformBlockiv");
  if (!program) {
    return error::kNoError;
  }
  GLuint service_id = program->service_id();
  GLint link_status = GL_FALSE;
  api()->glGetProgramivFn(service_id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
        "glGetActiveActiveUniformBlockiv", "program not linked");
    return error::kNoError;
  }
  if (index >= program->uniform_block_size_info().size()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glGetActiveUniformBlockiv",
                       "uniformBlockIndex >= active uniform blocks");
    return error::kNoError;
  }
  GLsizei num_values = 1;
  if (pname == GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES) {
    GLint num = 0;
    api()->glGetActiveUniformBlockivFn(service_id, index,
                                       GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &num);
    GLenum error = api()->glGetErrorFn();
    if (error != GL_NO_ERROR) {
      LOCAL_SET_GL_ERROR(error, "GetActiveUniformBlockiv", "");
      return error::kNoError;
    }
    num_values = static_cast<GLsizei>(num);
  }
  typedef cmds::GetActiveUniformBlockiv::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.params_shm_id, c.params_shm_offset, Result::ComputeSize(num_values));
  GLint* params = result ? result->GetData() : nullptr;
  if (params == nullptr) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  api()->glGetActiveUniformBlockivFn(service_id, index, pname, params);
  result->SetNumResults(num_values);
  return error::kNoError;
}
