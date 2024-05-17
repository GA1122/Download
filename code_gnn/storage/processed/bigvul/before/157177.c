void WebMediaPlayerImpl::BecameDominantVisibleContent(bool isDominant) {
  if (observer_)
    observer_->OnBecameDominantVisibleContent(isDominant);
}
