void HTMLMediaElement::removeAudioTrack(WebMediaPlayer::TrackId trackId) {
  BLINK_MEDIA_LOG << "removeAudioTrack(" << (void*)this << ")";

  audioTracks().remove(trackId);
}
