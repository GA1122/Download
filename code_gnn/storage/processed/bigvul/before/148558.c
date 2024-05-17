void WebContentsImpl::SetHasPersistentVideo(bool has_persistent_video) {
  if (has_persistent_video_ == has_persistent_video)
    return;

  has_persistent_video_ = has_persistent_video;
  NotifyPreferencesChanged();
  media_web_contents_observer()->RequestPersistentVideo(has_persistent_video);
}
