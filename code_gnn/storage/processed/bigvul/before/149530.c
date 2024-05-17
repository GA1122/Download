void LoadingPredictor::MaybeRemovePreconnect(const GURL& url) {
  DCHECK(!shutdown_);
  if (!preconnect_manager_)
    return;

  preconnect_manager_->Stop(url);
}
