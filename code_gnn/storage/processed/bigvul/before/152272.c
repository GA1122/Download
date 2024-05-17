void RenderFrameImpl::DidEnforceInsecureRequestPolicy(
    blink::WebInsecureRequestPolicy policy) {
  GetFrameHost()->EnforceInsecureRequestPolicy(policy);
}
