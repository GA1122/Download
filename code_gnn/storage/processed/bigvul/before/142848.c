void HTMLMediaElement::OnVisibilityChangedForLazyLoad(bool is_visible) {
  if (!is_visible || !web_media_player_)
    return;

  web_media_player_->OnBecameVisible();
  lazy_load_visibility_observer_->Stop();
  lazy_load_visibility_observer_ = nullptr;
}
