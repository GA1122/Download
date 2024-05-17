void WebMediaPlayerImpl::RegisterPictureInPictureWindowResizeCallback(
    blink::WebMediaPlayer::PipWindowResizedCallback callback) {
  DCHECK(IsInPictureInPicture() && !client_->IsInAutoPIP());

  delegate_->RegisterPictureInPictureWindowResizeCallback(delegate_id_,
                                                          std::move(callback));
}
