void HTMLMediaElement::removeVideoTrack(WebMediaPlayer::TrackId trackId) {
  BLINK_MEDIA_LOG << "removeVideoTrack(" << (void*)this << ")";

  videoTracks().remove(trackId);
}
