void HTMLMediaElement::RequestPlay() {
  autoplay_policy_->EnsureAutoplayInitiatedSet();
  PlayInternal();
}
