void RenderViewImpl::PlayerGone(WebKit::WebMediaPlayer* player) {
  DidPause(player);
}
