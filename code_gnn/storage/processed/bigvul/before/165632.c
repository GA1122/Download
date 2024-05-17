void Location::setProtocol(LocalDOMWindow* current_window,
                           LocalDOMWindow* entered_window,
                           const String& protocol,
                           ExceptionState& exception_state) {
  KURL url = GetDocument()->Url();
  if (!url.SetProtocol(protocol)) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kSyntaxError,
        "'" + protocol + "' is an invalid protocol.");
    return;
  }
  SetLocation(url.GetString(), current_window, entered_window,
              &exception_state);
}
