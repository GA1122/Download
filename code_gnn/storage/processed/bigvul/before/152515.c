void RenderFrameImpl::SendFailedProvisionalLoad(const std::string& http_method,
                                                const WebURLError& error,
                                                blink::WebLocalFrame* frame) {
  bool show_repost_interstitial =
      error.reason() == net::ERR_CACHE_MISS && http_method == "POST";

  FrameHostMsg_DidFailProvisionalLoadWithError_Params params;
  params.error_code = error.reason();
  GetContentClient()->renderer()->GetErrorDescription(
      error, http_method, &params.error_description);
  params.url = error.url(),
  params.showing_repost_interstitial = show_repost_interstitial;
  Send(new FrameHostMsg_DidFailProvisionalLoadWithError(routing_id_, params));
}
