void HTMLMediaElement::AudioTracksTimerFired(TimerBase*) {
  Vector<WebMediaPlayer::TrackId> enabled_track_ids;
  for (unsigned i = 0; i < audioTracks().length(); ++i) {
    AudioTrack* track = audioTracks().AnonymousIndexedGetter(i);
    if (track->enabled())
      enabled_track_ids.push_back(track->id());
  }

  GetWebMediaPlayer()->EnabledAudioTracksChanged(enabled_track_ids);
}
