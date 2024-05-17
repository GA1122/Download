ScriptPromise WebGLRenderingContextBase::makeXRCompatible(
    ScriptState* script_state) {
  if (isContextLost()) {
    return ScriptPromise::RejectWithDOMException(
        script_state, DOMException::Create(DOMExceptionCode::kInvalidStateError,
                                           "Context lost."));
  }

  if (xr_compatible_) {
    return ScriptPromise::CastUndefined(script_state);
  }

  if (ContextCreatedOnXRCompatibleAdapter()) {
    xr_compatible_ = true;
    return ScriptPromise::CastUndefined(script_state);
  }

  return ScriptPromise::RejectWithDOMException(
      script_state,
      DOMException::Create(
          DOMExceptionCode::kNotSupportedError,
          "Context is not compatible. Switching not yet implemented."));
}
