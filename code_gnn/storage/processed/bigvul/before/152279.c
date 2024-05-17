void RenderFrameImpl::DidLoadResourceFromMemoryCache(
    const blink::WebURLRequest& request,
    const blink::WebURLResponse& response) {
  if (request.Url().ProtocolIs(url::kDataScheme))
    return;

  Send(new FrameHostMsg_DidLoadResourceFromMemoryCache(
      routing_id_, request.Url(), request.HttpMethod().Utf8(),
      response.MimeType().Utf8(), request.TopFrameOrigin(),
      WebURLRequestToResourceType(request)));
}
