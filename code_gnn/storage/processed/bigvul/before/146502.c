ScriptValue WebGLRenderingContextBase::getBufferParameter(
    ScriptState* script_state,
    GLenum target,
    GLenum pname) {
  if (isContextLost() || !ValidateBufferTarget("getBufferParameter", target))
    return ScriptValue::CreateNull(script_state);

  switch (pname) {
    case GL_BUFFER_USAGE: {
      GLint value = 0;
      ContextGL()->GetBufferParameteriv(target, pname, &value);
      return WebGLAny(script_state, static_cast<unsigned>(value));
    }
    case GL_BUFFER_SIZE: {
      GLint value = 0;
      ContextGL()->GetBufferParameteriv(target, pname, &value);
      if (!IsWebGL2OrHigher())
        return WebGLAny(script_state, value);
      return WebGLAny(script_state, static_cast<GLint64>(value));
    }
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getBufferParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
