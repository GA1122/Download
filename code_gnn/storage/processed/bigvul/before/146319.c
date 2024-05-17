ScriptValue WebGLRenderingContextBase::GetWebGLIntArrayParameter(
    ScriptState* script_state,
    GLenum pname) {
  GLint value[4] = {0};
  if (!isContextLost())
    ContextGL()->GetIntegerv(pname, value);
  unsigned length = 0;
  switch (pname) {
    case GL_MAX_VIEWPORT_DIMS:
      length = 2;
      break;
    case GL_SCISSOR_BOX:
    case GL_VIEWPORT:
      length = 4;
      break;
    default:
      NOTIMPLEMENTED();
  }
  return WebGLAny(script_state, DOMInt32Array::Create(value, length));
}
