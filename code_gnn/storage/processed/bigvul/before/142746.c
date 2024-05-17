void HTMLMediaElement::AudioTrackChanged(AudioTrack* track) {
  BLINK_MEDIA_LOG << "audioTrackChanged(" << (void*)this
                  << ") trackId= " << String(track->id())
                  << " enabled=" << BoolString(track->enabled());
  DCHECK(MediaTracksEnabledInternally());

  audioTracks().ScheduleChangeEvent();

  if (media_source_)
    media_source_->OnTrackChanged(track);

  if (!audio_tracks_timer_.IsActive())
    audio_tracks_timer_.StartOneShot(TimeDelta(), FROM_HERE);
}
