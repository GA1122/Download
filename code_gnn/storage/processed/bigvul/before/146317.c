ScriptValue WebGLRenderingContextBase::GetUnsignedIntParameter(
    ScriptState* script_state,
    GLenum pname) {
  GLint value = 0;
  if (!isContextLost())
    ContextGL()->GetIntegerv(pname, &value);
  return WebGLAny(script_state, static_cast<unsigned>(value));
}
