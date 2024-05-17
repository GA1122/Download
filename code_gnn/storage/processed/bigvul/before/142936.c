void HTMLMediaElement::UpdatePlayState() {
  bool is_playing = GetWebMediaPlayer() && !GetWebMediaPlayer()->Paused();
  bool should_be_playing = PotentiallyPlaying();

  BLINK_MEDIA_LOG << "updatePlayState(" << (void*)this
                  << ") - shouldBePlaying = " << BoolString(should_be_playing)
                  << ", isPlaying = " << BoolString(is_playing);

  if (should_be_playing && !muted_)
    was_always_muted_ = false;

  if (should_be_playing) {
    SetDisplayMode(kVideo);

    if (!is_playing) {
      GetWebMediaPlayer()->SetRate(playbackRate());
      GetWebMediaPlayer()->SetVolume(EffectiveMediaVolume());
      GetWebMediaPlayer()->Play();
    }

    StartPlaybackProgressTimer();
    playing_ = true;
  } else {   
    if (is_playing) {
      GetWebMediaPlayer()->Pause();
    }

    playback_progress_timer_.Stop();
    playing_ = false;
    double time = currentTime();
    if (time > last_seek_time_)
      AddPlayedRange(last_seek_time_, time);
  }

  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}
