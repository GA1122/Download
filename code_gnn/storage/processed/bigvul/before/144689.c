void WebContentsImpl::StopMediaSession() {
  MediaSession::Get(this)->Stop();
}
