String DOMWindow::CrossDomainAccessErrorMessage(
    const LocalDOMWindow* calling_window) const {
  if (!calling_window || !calling_window->document() || !GetFrame())
    return String();

  const KURL& calling_window_url = calling_window->document()->Url();
  if (calling_window_url.IsNull())
    return String();

  const SecurityOrigin* active_origin =
      calling_window->document()->GetSecurityOrigin();
  const SecurityOrigin* target_origin =
      GetFrame()->GetSecurityContext()->GetSecurityOrigin();
  DCHECK(GetFrame()->IsRemoteFrame() ||
         !active_origin->CanAccess(target_origin));

  String message = "Blocked a frame with origin \"" +
                   active_origin->ToString() +
                   "\" from accessing a frame with origin \"" +
                   target_origin->ToString() + "\". ";

  KURL active_url = calling_window->document()->Url();
  KURL target_url = IsLocalDOMWindow()
                        ? blink::ToLocalDOMWindow(this)->document()->Url()
                        : KURL(NullURL(), target_origin->ToString());
  if (GetFrame()->GetSecurityContext()->IsSandboxed(kSandboxOrigin) ||
      calling_window->document()->IsSandboxed(kSandboxOrigin)) {
    message = "Blocked a frame at \"" +
              SecurityOrigin::Create(active_url)->ToString() +
              "\" from accessing a frame at \"" +
              SecurityOrigin::Create(target_url)->ToString() + "\". ";
    if (GetFrame()->GetSecurityContext()->IsSandboxed(kSandboxOrigin) &&
        calling_window->document()->IsSandboxed(kSandboxOrigin))
      return "Sandbox access violation: " + message +
             " Both frames are sandboxed and lack the \"allow-same-origin\" "
             "flag.";
    if (GetFrame()->GetSecurityContext()->IsSandboxed(kSandboxOrigin))
      return "Sandbox access violation: " + message +
             " The frame being accessed is sandboxed and lacks the "
             "\"allow-same-origin\" flag.";
    return "Sandbox access violation: " + message +
           " The frame requesting access is sandboxed and lacks the "
           "\"allow-same-origin\" flag.";
  }

  if (target_origin->Protocol() != active_origin->Protocol())
    return message + " The frame requesting access has a protocol of \"" +
           active_url.Protocol() +
           "\", the frame being accessed has a protocol of \"" +
           target_url.Protocol() + "\". Protocols must match.\n";

  if (target_origin->DomainWasSetInDOM() && active_origin->DomainWasSetInDOM())
    return message +
           "The frame requesting access set \"document.domain\" to \"" +
           active_origin->Domain() +
           "\", the frame being accessed set it to \"" +
           target_origin->Domain() +
           "\". Both must set \"document.domain\" to the same value to allow "
           "access.";
  if (active_origin->DomainWasSetInDOM())
    return message +
           "The frame requesting access set \"document.domain\" to \"" +
           active_origin->Domain() +
           "\", but the frame being accessed did not. Both must set "
           "\"document.domain\" to the same value to allow access.";
  if (target_origin->DomainWasSetInDOM())
    return message + "The frame being accessed set \"document.domain\" to \"" +
           target_origin->Domain() +
           "\", but the frame requesting access did not. Both must set "
           "\"document.domain\" to the same value to allow access.";

  return message + "Protocols, domains, and ports must match.";
}
