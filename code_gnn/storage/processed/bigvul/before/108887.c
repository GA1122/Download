static WebReferrerPolicy GetReferrerPolicyFromRequest(
    WebFrame* frame,
    const WebURLRequest& request) {
  return request.extraData() ?
      static_cast<RequestExtraData*>(request.extraData())->referrer_policy() :
      frame->document().referrerPolicy();
}
