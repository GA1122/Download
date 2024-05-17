void LockScreenMediaControlsView::SeekTo(double seek_progress) {
  DCHECK(position_.has_value());

  media_controller_remote_->SeekTo(seek_progress * position_->duration());
}
