void WebMediaPlayerImpl::OnPictureInPictureControlClicked(
    const std::string& control_id) {
  if (client_ && IsInPictureInPicture()) {
    client_->PictureInPictureControlClicked(
        blink::WebString::FromUTF8(control_id));
  }
}
