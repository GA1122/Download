void GLES2DecoderImpl::DoBindFragmentInputLocationCHROMIUM(
    GLuint program_id,
    GLint location,
    const std::string& name) {
  static const char kFunctionName[] = "glBindFragmentInputLocationCHROMIUM";
  if (!StringIsValidForGLES(name)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "invalid character");
    return;
  }
  if (ProgramManager::HasBuiltInPrefix(name)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName, "reserved prefix");
    return;
  }
  Program* program = GetProgram(program_id);
  if (!program || program->IsDeleted()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName, "invalid program");
    return;
  }
  if (location < 0 ||
      static_cast<uint32_t>(location) >= group_->max_varying_vectors() * 4) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                       "location out of range");
    return;
  }

  program->SetFragmentInputLocationBinding(name, location);
}
