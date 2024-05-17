void WebContentsImpl::ResumeMediaSession() {
  MediaSession::Get(this)->Resume();
}
