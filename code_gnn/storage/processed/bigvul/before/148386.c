std::string WebContentsImpl::GetDefaultMediaDeviceID(MediaStreamType type) {
  if (!delegate_)
    return std::string();
  return delegate_->GetDefaultMediaDeviceID(this, type);
}
