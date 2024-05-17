void HTMLMediaElement::RemoveAudioTrack(WebMediaPlayer::TrackId track_id) {
  BLINK_MEDIA_LOG << "removeAudioTrack(" << (void*)this << ")";

  audioTracks().Remove(track_id);
}
