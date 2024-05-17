void WebMediaPlayerMS::OnFrameSinkDestroyed() {
  bridge_->ClearSurfaceId();
}
