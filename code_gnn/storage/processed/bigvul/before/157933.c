Referrer RenderViewImpl::GetReferrerFromRequest(
    WebFrame* frame,
    const WebURLRequest& request) {
  return Referrer(blink::WebStringToGURL(
                      request.HttpHeaderField(WebString::FromUTF8("Referer"))),
                  request.GetReferrerPolicy());
}
