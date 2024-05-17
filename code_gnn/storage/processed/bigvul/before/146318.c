ScriptValue WebGLRenderingContextBase::GetWebGLFloatArrayParameter(
    ScriptState* script_state,
    GLenum pname) {
  GLfloat value[4] = {0};
  if (!isContextLost())
    ContextGL()->GetFloatv(pname, value);
  unsigned length = 0;
  switch (pname) {
    case GL_ALIASED_POINT_SIZE_RANGE:
    case GL_ALIASED_LINE_WIDTH_RANGE:
    case GL_DEPTH_RANGE:
      length = 2;
      break;
    case GL_BLEND_COLOR:
    case GL_COLOR_CLEAR_VALUE:
      length = 4;
      break;
    default:
      NOTIMPLEMENTED();
  }
  return WebGLAny(script_state, DOMFloat32Array::Create(value, length));
}
