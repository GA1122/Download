ScriptValue WebGLRenderingContextBase::getRenderbufferParameter(
    ScriptState* script_state,
    GLenum target,
    GLenum pname) {
  if (isContextLost())
    return ScriptValue::CreateNull(script_state);
  if (target != GL_RENDERBUFFER) {
    SynthesizeGLError(GL_INVALID_ENUM, "getRenderbufferParameter",
                      "invalid target");
    return ScriptValue::CreateNull(script_state);
  }
  if (!renderbuffer_binding_ || !renderbuffer_binding_->Object()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getRenderbufferParameter",
                      "no renderbuffer bound");
    return ScriptValue::CreateNull(script_state);
  }

  GLint value = 0;
  switch (pname) {
    case GL_RENDERBUFFER_SAMPLES:
      if (!IsWebGL2OrHigher()) {
        SynthesizeGLError(GL_INVALID_ENUM, "getRenderbufferParameter",
                          "invalid parameter name");
        return ScriptValue::CreateNull(script_state);
      }
    case GL_RENDERBUFFER_WIDTH:
    case GL_RENDERBUFFER_HEIGHT:
    case GL_RENDERBUFFER_RED_SIZE:
    case GL_RENDERBUFFER_GREEN_SIZE:
    case GL_RENDERBUFFER_BLUE_SIZE:
    case GL_RENDERBUFFER_ALPHA_SIZE:
    case GL_RENDERBUFFER_DEPTH_SIZE:
      ContextGL()->GetRenderbufferParameteriv(target, pname, &value);
      return WebGLAny(script_state, value);
    case GL_RENDERBUFFER_STENCIL_SIZE:
      ContextGL()->GetRenderbufferParameteriv(target, pname, &value);
      return WebGLAny(script_state, value);
    case GL_RENDERBUFFER_INTERNAL_FORMAT:
      return WebGLAny(script_state, renderbuffer_binding_->InternalFormat());
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getRenderbufferParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
