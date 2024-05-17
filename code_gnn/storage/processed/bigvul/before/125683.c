void RenderViewHostImpl::OnShowDesktopNotification(
    const ShowDesktopNotificationHostMsgParams& params) {
  GURL url = params.contents_url;
  if (params.is_html &&
      (url.SchemeIs(chrome::kJavaScriptScheme) ||
       url.SchemeIs(chrome::kFileScheme))) {
    return;
  }

  GetContentClient()->browser()->ShowDesktopNotification(
      params, GetProcess()->GetID(), GetRoutingID(), false);
}
