void WebMediaPlayerImpl::OnFrameSinkDestroyed() {
  bridge_->ClearSurfaceId();
}
