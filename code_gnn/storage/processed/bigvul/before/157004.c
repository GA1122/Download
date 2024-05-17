void WebMediaPlayerMS::SetPictureInPictureCustomControls(
    const std::vector<blink::PictureInPictureControlInfo>& controls) {
  delegate_->DidSetPictureInPictureCustomControls(delegate_id_, controls);
}
