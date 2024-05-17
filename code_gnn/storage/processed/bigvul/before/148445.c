void WebContentsImpl::MediaStartedPlaying(
    const WebContentsObserver::MediaPlayerInfo& media_info,
    const WebContentsObserver::MediaPlayerId& id) {
  if (media_info.has_video)
    currently_playing_video_count_++;

  for (auto& observer : observers_)
    observer.MediaStartedPlaying(media_info, id);
}
