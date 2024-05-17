void WebMediaPlayerMS::OnPictureInPictureModeEnded() {
  if (!client_ || !IsInPictureInPicture())
    return;

  client_->PictureInPictureStopped();
}
