void HTMLMediaElement::UpdatePlaybackRate() {
  if (web_media_player_ && PotentiallyPlaying())
    GetWebMediaPlayer()->SetRate(playbackRate());
}
