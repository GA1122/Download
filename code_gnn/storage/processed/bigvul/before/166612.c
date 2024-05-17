ScriptValue WebGL2RenderingContextBase::getSyncParameter(
    ScriptState* script_state,
    WebGLSync* sync,
    GLenum pname) {
  if (isContextLost() || !ValidateWebGLObject("getSyncParameter", sync))
    return ScriptValue::CreateNull(script_state);

  switch (pname) {
    case GL_OBJECT_TYPE:
    case GL_SYNC_STATUS:
    case GL_SYNC_CONDITION:
    case GL_SYNC_FLAGS: {
      sync->UpdateCache(ContextGL());
      return WebGLAny(script_state, sync->GetCachedResult(pname));
    }
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getSyncParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
