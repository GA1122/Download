void HTMLMediaElement::SelectedVideoTrackChanged(VideoTrack* track) {
  BLINK_MEDIA_LOG << "selectedVideoTrackChanged(" << (void*)this
                  << ") selectedTrackId="
                  << (track->selected() ? String(track->id()) : "none");
  DCHECK(MediaTracksEnabledInternally());

  if (track->selected())
    videoTracks().TrackSelected(track->id());

  videoTracks().ScheduleChangeEvent();

  if (media_source_)
    media_source_->OnTrackChanged(track);

  WebMediaPlayer::TrackId id = track->id();
  GetWebMediaPlayer()->SelectedVideoTrackChanged(track->selected() ? &id
                                                                   : nullptr);
}
