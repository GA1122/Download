error::Error GLES2DecoderPassthroughImpl::DoGetProgramResourceName(
    GLuint program,
    GLenum program_interface,
    GLuint index,
    std::string* name) {
  CheckErrorCallbackState();

  GLuint service_id = GetProgramServiceID(program, resources_);
  GLint max_name_length = 0;
  api()->glGetProgramInterfaceivFn(service_id, program_interface,
                                   GL_MAX_NAME_LENGTH, &max_name_length);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  std::vector<GLchar> buffer(max_name_length, 0);
  GLsizei length = 0;
  api()->glGetProgramResourceNameFn(service_id, program_interface, index,
                                    max_name_length, &length, buffer.data());
  DCHECK_LE(length, max_name_length);
  *name = length > 0 ? std::string(buffer.data(), length) : std::string();
  return error::kNoError;
}
