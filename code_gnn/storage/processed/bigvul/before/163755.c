bool DelegatedFrameHost::CanCopyFromCompositingSurface() const {
  return compositor_ &&
         client_->DelegatedFrameHostGetLayer()->has_external_content();
}
