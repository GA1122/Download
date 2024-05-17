void HTMLMediaElement::audioTrackChanged(AudioTrack* track) {
  BLINK_MEDIA_LOG << "audioTrackChanged(" << (void*)this
                  << ") trackId= " << String(track->id())
                  << " enabled=" << boolString(track->enabled());
  DCHECK(mediaTracksEnabledInternally());

  audioTracks().scheduleChangeEvent();

  if (m_mediaSource)
    m_mediaSource->onTrackChanged(track);

  if (!m_audioTracksTimer.isActive())
    m_audioTracksTimer.startOneShot(0, BLINK_FROM_HERE);
}
