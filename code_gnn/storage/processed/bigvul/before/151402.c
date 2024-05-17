FetchContext* FrameFetchContext::Detach() {
  if (IsDetached())
    return this;

  if (document_) {
    frozen_state_ = new FrozenState(
        GetReferrerPolicy(), GetOutgoingReferrer(), Url(), GetSecurityOrigin(),
        GetParentSecurityOrigin(), GetAddressSpace(),
        GetContentSecurityPolicy(), GetSiteForCookies(), GetRequestorOrigin(),
        GetRequestorOriginForFrameLoading(), GetClientHintsPreferences(),
        GetDevicePixelRatio(), GetUserAgent(), IsMainFrame(),
        IsSVGImageChromeClient());
  } else {
    frozen_state_ = new FrozenState(
        kReferrerPolicyDefault, String(), NullURL(), GetSecurityOrigin(),
        GetParentSecurityOrigin(), GetAddressSpace(),
        GetContentSecurityPolicy(), GetSiteForCookies(),
        SecurityOrigin::CreateUnique(), SecurityOrigin::CreateUnique(),
        GetClientHintsPreferences(), GetDevicePixelRatio(), GetUserAgent(),
        IsMainFrame(), IsSVGImageChromeClient());
  }

  document_ = nullptr;

  return this;
}