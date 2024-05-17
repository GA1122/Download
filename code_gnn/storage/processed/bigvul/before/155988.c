void InlineLoginHandlerImpl::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  if (!web_contents() ||
      !navigation_handle->HasCommitted() ||
      navigation_handle->IsErrorPage()) {
    return;
  }

  content::RenderFrameHost* gaia_frame =
      signin::GetAuthFrame(web_contents(), "signin-frame");
  if (navigation_handle->GetRenderFrameHost() != gaia_frame)
    return;

  const GURL kGaiaExtOrigin(
      GaiaUrls::GetInstance()->signin_completed_continue_url().GetOrigin());
  if (!navigation_handle->GetURL().is_empty()) {
    GURL origin(navigation_handle->GetURL().GetOrigin());
    if (navigation_handle->GetURL().spec() != url::kAboutBlankURL &&
        origin != kGaiaExtOrigin &&
        !gaia::IsGaiaSignonRealm(origin)) {
      confirm_untrusted_signin_ = true;
    }
  }
}
