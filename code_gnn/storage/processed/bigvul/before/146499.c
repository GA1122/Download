WebGLActiveInfo* WebGLRenderingContextBase::getActiveUniform(
    WebGLProgram* program,
    GLuint index) {
  if (isContextLost() || !ValidateWebGLObject("getActiveUniform", program))
    return nullptr;
  GLuint program_id = ObjectNonZero(program);
  GLint max_name_length = -1;
  ContextGL()->GetProgramiv(program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                            &max_name_length);
  if (max_name_length < 0)
    return nullptr;
  if (max_name_length == 0) {
    SynthesizeGLError(GL_INVALID_VALUE, "getActiveUniform",
                      "no active uniforms exist");
    return nullptr;
  }
  LChar* name_ptr;
  RefPtr<StringImpl> name_impl =
      StringImpl::CreateUninitialized(max_name_length, name_ptr);
  GLsizei length = 0;
  GLint size = -1;
  GLenum type = 0;
  ContextGL()->GetActiveUniform(program_id, index, max_name_length, &length,
                                &size, &type,
                                reinterpret_cast<GLchar*>(name_ptr));
  if (size < 0)
    return nullptr;
  return WebGLActiveInfo::Create(name_impl->Substring(0, length), type, size);
}
