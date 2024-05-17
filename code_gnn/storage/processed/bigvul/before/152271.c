void RenderFrameImpl::DidEnforceInsecureNavigationsSet(
    const std::vector<uint32_t>& set) {
  GetFrameHost()->EnforceInsecureNavigationsSet(set);
}
