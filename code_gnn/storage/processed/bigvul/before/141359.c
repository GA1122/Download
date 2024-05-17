void Document::setCookie(const String& value, ExceptionState& exception_state) {
  if (GetSettings() && !GetSettings()->GetCookieEnabled())
    return;

  UseCounter::Count(*this, WebFeature::kCookieSet);


  if (!GetSecurityOrigin()->CanAccessCookies()) {
    if (IsSandboxed(kSandboxOrigin))
      exception_state.ThrowSecurityError(
          "The document is sandboxed and lacks the 'allow-same-origin' flag.");
    else if (Url().ProtocolIs("data"))
      exception_state.ThrowSecurityError(
          "Cookies are disabled inside 'data:' URLs.");
    else
      exception_state.ThrowSecurityError("Access is denied for this document.");
    return;
  } else if (GetSecurityOrigin()->IsLocal()) {
    UseCounter::Count(*this, WebFeature::kFileAccessedCookies);
  }

  KURL cookie_url = CookieURL();
  if (cookie_url.IsEmpty())
    return;

  SetCookies(this, cookie_url, value);
}
