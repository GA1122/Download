ScriptValue WebGLRenderingContextBase::getTexParameter(
    ScriptState* script_state,
    GLenum target,
    GLenum pname) {
  if (isContextLost())
    return ScriptValue::CreateNull(script_state);
  if (!ValidateTextureBinding("getTexParameter", target))
    return ScriptValue::CreateNull(script_state);
  switch (pname) {
    case GL_TEXTURE_MAG_FILTER:
    case GL_TEXTURE_MIN_FILTER:
    case GL_TEXTURE_WRAP_S:
    case GL_TEXTURE_WRAP_T: {
      GLint value = 0;
      ContextGL()->GetTexParameteriv(target, pname, &value);
      return WebGLAny(script_state, static_cast<unsigned>(value));
    }
    case GL_TEXTURE_MAX_ANISOTROPY_EXT:   
      if (ExtensionEnabled(kEXTTextureFilterAnisotropicName)) {
        GLfloat value = 0.f;
        ContextGL()->GetTexParameterfv(target, pname, &value);
        return WebGLAny(script_state, value);
      }
      SynthesizeGLError(
          GL_INVALID_ENUM, "getTexParameter",
          "invalid parameter name, EXT_texture_filter_anisotropic not enabled");
      return ScriptValue::CreateNull(script_state);
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getTexParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
