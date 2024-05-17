void RenderViewImpl::loadURLExternally(
    WebFrame* frame, const WebURLRequest& request,
    WebNavigationPolicy policy,
    const WebString& suggested_name) {
  GURL referrer(request.httpHeaderField(WebString::fromUTF8("Referer")));
  if (policy == WebKit::WebNavigationPolicyDownload) {
    Send(new ViewHostMsg_DownloadUrl(routing_id_, request.url(), referrer,
                                     suggested_name));
  } else {
    OpenURL(frame, request.url(),
            Referrer(referrer, GetReferrerPolicyFromRequest(request)), policy);
  }
}
