ScriptPromise WebGLRenderingContextBase::setCompatibleXRDevice(
    ScriptState* script_state,
    XRDevice* xr_device) {

  if (isContextLost()) {
    return ScriptPromise::RejectWithDOMException(
        script_state,
        DOMException::Create(kInvalidStateError, "Context lost."));
  }

  if (xr_device == compatible_xr_device_) {
    return ScriptPromise::CastUndefined(script_state);
  }

  if (ContextCreatedOnCompatibleAdapter(xr_device)) {
    compatible_xr_device_ = xr_device;
    return ScriptPromise::CastUndefined(script_state);
  } else {
    return ScriptPromise::RejectWithDOMException(
        script_state,
        DOMException::Create(
            kNotSupportedError,
            "Context is not compatible. Switching not yet implemented."));
  }
}
