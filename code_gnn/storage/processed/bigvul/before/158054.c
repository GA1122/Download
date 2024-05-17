void LocalFrameClientImpl::DidEnforceInsecureRequestPolicy(
    WebInsecureRequestPolicy policy) {
  if (!web_frame_->Client())
    return;
  web_frame_->Client()->DidEnforceInsecureRequestPolicy(policy);
}
