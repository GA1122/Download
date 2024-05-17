void RenderViewHostImpl::LoadStateChanged(
    const GURL& url,
    const net::LoadStateWithParam& load_state,
    uint64 upload_position,
    uint64 upload_size) {
  delegate_->LoadStateChanged(url, load_state, upload_position, upload_size);
}
