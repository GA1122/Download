void RenderViewImpl::loadURLExternally(
    WebFrame* frame, const WebURLRequest& request,
    WebNavigationPolicy policy,
    const WebString& suggested_name) {
  Referrer referrer(
      GURL(request.httpHeaderField(WebString::fromUTF8("Referer"))),
      GetReferrerPolicyFromRequest(frame, request));
  if (policy == WebKit::WebNavigationPolicyDownload) {
    Send(new ViewHostMsg_DownloadUrl(routing_id_, request.url(), referrer,
                                     suggested_name));
  } else {
    OpenURL(frame, request.url(), referrer, policy);
  }
}
