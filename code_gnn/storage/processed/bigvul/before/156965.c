bool WebMediaPlayerMS::IsInPictureInPicture() const {
  DCHECK(client_);
  return (!client_->IsInAutoPIP() &&
          client_->DisplayType() ==
              WebMediaPlayer::DisplayType::kPictureInPicture);
}
