void WebMediaPlayerImpl::OnPictureInPictureModeEnded() {
  if (!client_ || !IsInPictureInPicture())
    return;

  client_->PictureInPictureStopped();
}
