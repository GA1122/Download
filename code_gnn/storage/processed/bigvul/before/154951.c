ScriptValue WebGLRenderingContextBase::GetBooleanParameter(
    ScriptState* script_state,
    GLenum pname) {
  GLboolean value = 0;
  if (!isContextLost())
    ContextGL()->GetBooleanv(pname, &value);
  return WebGLAny(script_state, static_cast<bool>(value));
}
