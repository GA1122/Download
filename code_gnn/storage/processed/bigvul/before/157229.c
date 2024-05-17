bool WebMediaPlayerImpl::IsInPictureInPicture() const {
  DCHECK(client_);
  return client_->DisplayType() ==
         WebMediaPlayer::DisplayType::kPictureInPicture;
}
