void LocalFrameClientImpl::DidAddContentSecurityPolicies(
    const blink::WebVector<WebContentSecurityPolicy>& policies) {
  if (web_frame_->Client())
    web_frame_->Client()->DidAddContentSecurityPolicies(policies);
}
