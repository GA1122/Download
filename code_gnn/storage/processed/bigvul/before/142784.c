void HTMLMediaElement::DurationChanged() {
  BLINK_MEDIA_LOG << "durationChanged(" << (void*)this << ")";

  CHECK(web_media_player_);
  double new_duration = web_media_player_->Duration();

  DurationChanged(new_duration, CurrentPlaybackPosition() > new_duration);
}
