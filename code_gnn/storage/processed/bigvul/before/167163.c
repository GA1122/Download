base::Optional<ExceptionCode> HTMLMediaElement::Play() {
  BLINK_MEDIA_LOG << "play(" << (void*)this << ")";

  base::Optional<ExceptionCode> exception_code =
      autoplay_policy_->RequestPlay();

  if (exception_code == kNotAllowedError) {
    if (!paused_) {
      PlayInternal();
      return base::nullopt;
    }
    return exception_code;
  }

  autoplay_policy_->StopAutoplayMutedWhenVisible();

  if (error_ && error_->code() == MediaError::kMediaErrSrcNotSupported)
    return kNotSupportedError;

  DCHECK(!exception_code.has_value());

  PlayInternal();

  return base::nullopt;
}
