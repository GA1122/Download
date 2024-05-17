void WebMediaPlayerImpl::OnBufferingStateChange(BufferingState state) {
  OnBufferingStateChangeInternal(state, false);
}
