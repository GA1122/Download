error::Error GLES2DecoderPassthroughImpl::DoGetActiveAttrib(GLuint program,
                                                            GLuint index,
                                                            GLint* size,
                                                            GLenum* type,
                                                            std::string* name,
                                                            int32_t* success) {
  CheckErrorCallbackState();

  GLuint service_id = GetProgramServiceID(program, resources_);
  GLint active_attribute_max_length = 0;
  api()->glGetProgramivFn(service_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                          &active_attribute_max_length);
  if (CheckErrorCallbackState()) {
    *success = 0;
    return error::kNoError;
  }

  std::vector<char> name_buffer(active_attribute_max_length, 0);
  api()->glGetActiveAttribFn(service_id, index, name_buffer.size(), nullptr,
                             size, type, name_buffer.data());
  *name = std::string(name_buffer.data());
  *success = CheckErrorCallbackState() ? 0 : 1;
  return error::kNoError;
}
