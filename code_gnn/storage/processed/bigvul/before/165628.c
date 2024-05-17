void Location::setHost(LocalDOMWindow* current_window,
                       LocalDOMWindow* entered_window,
                       const String& host,
                       ExceptionState& exception_state) {
  KURL url = GetDocument()->Url();
  url.SetHostAndPort(host);
  SetLocation(url.GetString(), current_window, entered_window,
              &exception_state);
}
