bool DOMWindow::IsInsecureScriptAccess(LocalDOMWindow& calling_window,
                                       const KURL& url) {
  if (!url.ProtocolIsJavaScript())
    return false;

  if (IsCurrentlyDisplayedInFrame()) {
    if (&calling_window == this)
      return false;

    if (calling_window.document()->GetSecurityOrigin()->CanAccess(
            GetFrame()->GetSecurityContext()->GetSecurityOrigin())) {
      return false;
    }
  }

  calling_window.PrintErrorMessage(
      CrossDomainAccessErrorMessage(&calling_window));
  return true;
}
