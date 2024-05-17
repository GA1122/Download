String DOMWindow::SanitizedCrossDomainAccessErrorMessage(
    const LocalDOMWindow* calling_window) const {
  if (!calling_window || !calling_window->document() || !GetFrame())
    return String();

  const KURL& calling_window_url = calling_window->document()->Url();
  if (calling_window_url.IsNull())
    return String();

  const SecurityOrigin* active_origin =
      calling_window->document()->GetSecurityOrigin();
  String message = "Blocked a frame with origin \"" +
                   active_origin->ToString() +
                   "\" from accessing a cross-origin frame.";


  return message;
}
