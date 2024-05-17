void RenderFrameImpl::SendFailedProvisionalLoad(
    const blink::WebURLRequest& request,
    const blink::WebURLError& error,
    blink::WebLocalFrame* frame) {
  bool show_repost_interstitial =
      (error.reason == net::ERR_CACHE_MISS &&
       base::EqualsASCII(base::StringPiece16(request.httpMethod()), "POST"));

  FrameHostMsg_DidFailProvisionalLoadWithError_Params params;
  params.error_code = error.reason;
  GetContentClient()->renderer()->GetNavigationErrorStrings(
      render_view_.get(), frame, request, error, NULL,
      &params.error_description);
  params.url = error.unreachableURL;
  params.showing_repost_interstitial = show_repost_interstitial;
  params.was_ignored_by_handler = error.wasIgnoredByHandler;
  Send(new FrameHostMsg_DidFailProvisionalLoadWithError(routing_id_, params));
}
