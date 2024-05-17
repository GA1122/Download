error::Error GLES2DecoderImpl::DoBindFragDataLocationIndexed(
    GLuint program_id,
    GLuint colorName,
    GLuint index,
    const std::string& name) {
  const char kFunctionName[] = "glBindFragDataLocationIndexEXT";
  if (!StringIsValidForGLES(name)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "invalid character");
    return error::kNoError;
  }
  if (ProgramManager::HasBuiltInPrefix(name)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName, "reserved prefix");
    return error::kNoError;
  }
  if (index != 0 && index != 1) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "index out of range");
    return error::kNoError;
  }
  if ((index == 0 && colorName >= group_->max_draw_buffers()) ||
      (index == 1 && colorName >= group_->max_dual_source_draw_buffers())) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                       "colorName out of range for the color index");
    return error::kNoError;
  }
  Program* program = GetProgramInfoNotShader(program_id, kFunctionName);
  if (!program) {
    return error::kNoError;
  }
  program->SetProgramOutputLocationIndexedBinding(name, colorName, index);
  return error::kNoError;
}
