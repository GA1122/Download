ScriptValue WebGLRenderingContextBase::GetFloatParameter(
    ScriptState* script_state,
    GLenum pname) {
  GLfloat value = 0;
  if (!isContextLost())
    ContextGL()->GetFloatv(pname, &value);
  return WebGLAny(script_state, value);
}
