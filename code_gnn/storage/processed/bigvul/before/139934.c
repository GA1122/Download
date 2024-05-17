void HTMLMediaElement::audioTracksTimerFired(TimerBase*) {
  Vector<WebMediaPlayer::TrackId> enabledTrackIds;
  for (unsigned i = 0; i < audioTracks().length(); ++i) {
    AudioTrack* track = audioTracks().anonymousIndexedGetter(i);
    if (track->enabled())
      enabledTrackIds.push_back(track->id());
  }

  webMediaPlayer()->enabledAudioTracksChanged(enabledTrackIds);
}
