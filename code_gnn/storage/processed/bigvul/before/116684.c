void RenderViewImpl::didLoadResourceFromMemoryCache(
    WebFrame* frame, const WebURLRequest& request,
    const WebURLResponse& response) {
  GURL url(request.url());
  if (url.SchemeIs("data"))
    return;

  Send(new ViewHostMsg_DidLoadResourceFromMemoryCache(
      routing_id_,
      url,
      response.securityInfo(),
      request.httpMethod().utf8(),
      ResourceType::FromTargetType(request.targetType())));
}
