error::Error GLES2DecoderPassthroughImpl::DoGetActiveUniformBlockName(
    GLuint program,
    GLuint index,
    std::string* name) {
  CheckErrorCallbackState();

  GLuint program_service_id = GetProgramServiceID(program, resources_);
  GLint max_name_length = 0;
  api()->glGetProgramivFn(program_service_id,
                          GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,
                          &max_name_length);

  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  std::vector<GLchar> buffer(max_name_length, 0);
  GLsizei length = 0;
  api()->glGetActiveUniformBlockNameFn(program_service_id, index,
                                       max_name_length, &length, buffer.data());
  DCHECK(length <= max_name_length);
  *name = length > 0 ? std::string(buffer.data(), length) : std::string();

  return error::kNoError;
}
