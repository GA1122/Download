void WebContentsImpl::SetHasPictureInPictureVideo(
    bool has_picture_in_picture_video) {
  if (has_picture_in_picture_video == has_picture_in_picture_video_)
    return;
  has_picture_in_picture_video_ = has_picture_in_picture_video;
  NotifyNavigationStateChanged(INVALIDATE_TYPE_TAB);
  for (auto& observer : observers_)
    observer.MediaPictureInPictureChanged(has_picture_in_picture_video_);
}
