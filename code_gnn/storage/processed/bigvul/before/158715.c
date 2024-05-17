error::Error GLES2DecoderImpl::HandleGetActiveUniformsiv(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::GetActiveUniformsiv& c =
      *static_cast<const volatile gles2::cmds::GetActiveUniformsiv*>(cmd_data);
  GLuint program_id = c.program;
  GLenum pname = static_cast<GLenum>(c.pname);
  Bucket* bucket = GetBucket(c.indices_bucket_id);
  if (!bucket) {
    return error::kInvalidArguments;
  }
  if (!validators_->uniform_parameter.IsValid(pname)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glGetActiveUniformsiv", pname, "pname");
    return error::kNoError;
  }
  GLsizei count = static_cast<GLsizei>(bucket->size() / sizeof(GLuint));
  const GLuint* indices = bucket->GetDataAs<const GLuint*>(0, bucket->size());
  typedef cmds::GetActiveUniformsiv::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.params_shm_id, c.params_shm_offset, Result::ComputeSize(count));
  GLint* params = result ? result->GetData() : nullptr;
  if (params == nullptr) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  Program* program = GetProgramInfoNotShader(
      program_id, "glGetActiveUniformsiv");
  if (!program) {
    return error::kNoError;
  }
  GLint activeUniforms = 0;
  program->GetProgramiv(GL_ACTIVE_UNIFORMS, &activeUniforms);
  for (int i = 0; i < count; i++) {
    if (indices[i] >= static_cast<GLuint>(activeUniforms)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE,
          "glGetActiveUniformsiv", "index >= active uniforms");
      return error::kNoError;
    }
  }
  GLuint service_id = program->service_id();
  GLint link_status = GL_FALSE;
  api()->glGetProgramivFn(service_id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
        "glGetActiveUniformsiv", "program not linked");
    return error::kNoError;
  }
  api()->glGetActiveUniformsivFn(service_id, count, indices, pname, params);
  result->SetNumResults(count);
  return error::kNoError;
}
