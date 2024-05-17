void HTMLMediaElement::selectedVideoTrackChanged(VideoTrack* track) {
  BLINK_MEDIA_LOG << "selectedVideoTrackChanged(" << (void*)this
                  << ") selectedTrackId="
                  << (track->selected() ? String(track->id()) : "none");
  DCHECK(mediaTracksEnabledInternally());

  if (track->selected())
    videoTracks().trackSelected(track->id());

  videoTracks().scheduleChangeEvent();

  if (m_mediaSource)
    m_mediaSource->onTrackChanged(track);

  WebMediaPlayer::TrackId id = track->id();
  webMediaPlayer()->selectedVideoTrackChanged(track->selected() ? &id
                                                                : nullptr);
}
