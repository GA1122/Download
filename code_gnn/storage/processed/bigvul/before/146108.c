WebGLActiveInfo* WebGL2RenderingContextBase::getTransformFeedbackVarying(
    WebGLProgram* program,
    GLuint index) {
  if (isContextLost() ||
      !ValidateWebGLObject("getTransformFeedbackVarying", program))
    return nullptr;

  if (!program->LinkStatus(this)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getTransformFeedbackVarying",
                      "program not linked");
    return nullptr;
  }
  GLint max_index = 0;
  ContextGL()->GetProgramiv(ObjectOrZero(program),
                            GL_TRANSFORM_FEEDBACK_VARYINGS, &max_index);
  if (index >= static_cast<GLuint>(max_index)) {
    SynthesizeGLError(GL_INVALID_VALUE, "getTransformFeedbackVarying",
                      "invalid index");
    return nullptr;
  }

  GLint max_name_length = -1;
  ContextGL()->GetProgramiv(ObjectOrZero(program),
                            GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,
                            &max_name_length);
  if (max_name_length <= 0) {
    return nullptr;
  }
  std::unique_ptr<GLchar[]> name = WrapArrayUnique(new GLchar[max_name_length]);
  GLsizei length = 0;
  GLsizei size = 0;
  GLenum type = 0;
  ContextGL()->GetTransformFeedbackVarying(ObjectOrZero(program), index,
                                           max_name_length, &length, &size,
                                           &type, name.get());

  if (length == 0 || size == 0 || type == 0) {
    return nullptr;
  }

  return WebGLActiveInfo::Create(String(name.get(), length), type, size);
}
