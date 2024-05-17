void WebMediaPlayerImpl::RegisterPictureInPictureWindowResizeCallback(
    blink::WebMediaPlayer::PipWindowResizedCallback callback) {
  DCHECK(client_->DisplayType() ==
             WebMediaPlayer::DisplayType::kPictureInPicture &&
         !client_->IsInAutoPIP());

  delegate_->RegisterPictureInPictureWindowResizeCallback(delegate_id_,
                                                          std::move(callback));
}
