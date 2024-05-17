void WebMediaPlayerImpl::OnPictureInPictureModeEnded() {
  if (!client_ || client_->DisplayType() !=
                      WebMediaPlayer::DisplayType::kPictureInPicture) {
    return;
  }

  client_->PictureInPictureStopped();
}
