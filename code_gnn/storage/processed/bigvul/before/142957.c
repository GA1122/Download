void HTMLMediaElement::load() {
  BLINK_MEDIA_LOG << "load(" << (void*)this << ")";

  autoplay_policy_->TryUnlockingUserGesture();

  ignore_preload_none_ = true;
  InvokeLoadAlgorithm();
}
