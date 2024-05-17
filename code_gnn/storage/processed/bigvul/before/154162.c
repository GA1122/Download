error::Error GLES2DecoderImpl::GetFragDataIndexHelper(
    GLuint program_id,
    uint32_t index_shm_id,
    uint32_t index_shm_offset,
    const std::string& name_str) {
  const char kFunctionName[] = "glGetFragDataIndexEXT";
  GLint* index =
      GetSharedMemoryAs<GLint*>(index_shm_id, index_shm_offset, sizeof(GLint));
  if (!index) {
    return error::kOutOfBounds;
  }
  if (*index != -1) {
    return error::kInvalidArguments;
  }
  Program* program = GetProgramInfoNotShader(program_id, kFunctionName);
  if (!program) {
    return error::kNoError;
  }
  if (!program->IsValid()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                       "program not linked");
    return error::kNoError;
  }

  *index = program->GetFragDataIndex(name_str);
  return error::kNoError;
}
