ScriptValue WebGLRenderingContextBase::GetIntParameter(
    ScriptState* script_state,
    GLenum pname) {
  GLint value = 0;
  if (!isContextLost()) {
    ContextGL()->GetIntegerv(pname, &value);
    switch (pname) {
      case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
      case GL_IMPLEMENTATION_COLOR_READ_TYPE:
        if (value == 0) {
          return ScriptValue::CreateNull(script_state);
        }
        break;
      default:
        break;
    }
  }
  return WebGLAny(script_state, value);
}
