void WebMediaPlayerImpl::OnPictureInPictureControlClicked(
    const std::string& control_id) {
  if (client_ && client_->DisplayType() ==
                     WebMediaPlayer::DisplayType::kPictureInPicture) {
    client_->PictureInPictureControlClicked(
        blink::WebString::FromUTF8(control_id));
  }
}
