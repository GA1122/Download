void ScriptProcessorHandler::SetChannelCountMode(
    const String& mode,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(Context());

  if ((mode == "max") || (mode == "clamped-max")) {
    exception_state.ThrowDOMException(
        kNotSupportedError,
        "channelCountMode cannot be changed from 'explicit' to '" + mode + "'");
  }
}
