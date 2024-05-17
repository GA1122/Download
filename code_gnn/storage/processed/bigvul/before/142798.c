void HTMLMediaElement::ForgetResourceSpecificTracks() {
  if (text_tracks_) {
    TrackDisplayUpdateScope scope(GetCueTimeline());
    text_tracks_->RemoveAllInbandTracks();
  }

  audio_tracks_->RemoveAll();
  video_tracks_->RemoveAll();

  audio_tracks_timer_.Stop();
}
