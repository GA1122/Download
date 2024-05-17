static WebReferrerPolicy GetReferrerPolicyFromRequest(
    const WebURLRequest& request) {
  return request.extraData() ?
      static_cast<RequestExtraData*>(request.extraData())->referrer_policy() :
      WebKit::WebReferrerPolicyDefault;
}
