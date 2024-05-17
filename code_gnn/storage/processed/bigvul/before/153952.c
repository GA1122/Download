error::Error GLES2DecoderImpl::DoBindFragDataLocation(GLuint program_id,
                                                      GLuint colorName,
                                                      const std::string& name) {
  const char kFunctionName[] = "glBindFragDataLocationEXT";
  if (!StringIsValidForGLES(name)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "invalid character");
    return error::kNoError;
  }
  if (ProgramManager::HasBuiltInPrefix(name)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName, "reserved prefix");
    return error::kNoError;
  }
  if (colorName >= group_->max_draw_buffers()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                       "colorName out of range");
    return error::kNoError;
  }
  Program* program = GetProgramInfoNotShader(program_id, kFunctionName);
  if (!program) {
    return error::kNoError;
  }
  program->SetProgramOutputLocationBinding(name, colorName);
  return error::kNoError;
}
