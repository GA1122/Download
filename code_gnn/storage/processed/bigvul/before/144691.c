void WebContentsImpl::SuspendMediaSession() {
  MediaSession::Get(this)->Suspend();
}
