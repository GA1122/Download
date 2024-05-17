WebContextProxy::SessionCookieMode WebContext::sessionCookieMode() const {
  if (IsInitialized()) {
    return static_cast<SessionCookieMode>(context_->GetSessionCookieMode());
  }

  return static_cast<SessionCookieMode>(construct_props_->session_cookie_mode);
}
