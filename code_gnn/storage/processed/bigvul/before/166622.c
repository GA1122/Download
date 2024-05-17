void WebGL2RenderingContextBase::transformFeedbackVaryings(
    WebGLProgram* program,
    const Vector<String>& varyings,
    GLenum buffer_mode) {
  if (isContextLost() ||
      !ValidateWebGLObject("transformFeedbackVaryings", program))
    return;

  switch (buffer_mode) {
    case GL_SEPARATE_ATTRIBS:
      if (varyings.size() >
          static_cast<size_t>(max_transform_feedback_separate_attribs_)) {
        SynthesizeGLError(GL_INVALID_VALUE, "transformFeedbackVaryings",
                          "too many varyings");
        return;
      }
      break;
    case GL_INTERLEAVED_ATTRIBS:
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "transformFeedbackVaryings",
                        "invalid buffer mode");
      return;
  }

  Vector<CString> keep_alive;   
  Vector<const char*> varying_strings;
  for (size_t i = 0; i < varyings.size(); ++i) {
    keep_alive.push_back(varyings[i].Ascii());
    varying_strings.push_back(keep_alive.back().data());
  }

  program->SetRequiredTransformFeedbackBufferCount(
      buffer_mode == GL_INTERLEAVED_ATTRIBS ? 1 : varyings.size());

  ContextGL()->TransformFeedbackVaryings(ObjectOrZero(program), varyings.size(),
                                         varying_strings.data(), buffer_mode);
}
