ScriptValue WebGL2RenderingContextBase::getTexParameter(
    ScriptState* script_state,
    GLenum target,
    GLenum pname) {
  if (isContextLost() || !ValidateTextureBinding("getTexParameter", target))
    return ScriptValue::CreateNull(script_state);

  switch (pname) {
    case GL_TEXTURE_WRAP_R:
    case GL_TEXTURE_COMPARE_FUNC:
    case GL_TEXTURE_COMPARE_MODE:
    case GL_TEXTURE_IMMUTABLE_LEVELS: {
      GLint value = 0;
      ContextGL()->GetTexParameteriv(target, pname, &value);
      return WebGLAny(script_state, static_cast<unsigned>(value));
    }
    case GL_TEXTURE_IMMUTABLE_FORMAT: {
      GLint value = 0;
      ContextGL()->GetTexParameteriv(target, pname, &value);
      return WebGLAny(script_state, static_cast<bool>(value));
    }
    case GL_TEXTURE_BASE_LEVEL:
    case GL_TEXTURE_MAX_LEVEL: {
      GLint value = 0;
      ContextGL()->GetTexParameteriv(target, pname, &value);
      return WebGLAny(script_state, value);
    }
    case GL_TEXTURE_MAX_LOD:
    case GL_TEXTURE_MIN_LOD: {
      GLfloat value = 0.f;
      ContextGL()->GetTexParameterfv(target, pname, &value);
      return WebGLAny(script_state, value);
    }
    default:
      return WebGLRenderingContextBase::getTexParameter(script_state, target,
                                                        pname);
  }
}
