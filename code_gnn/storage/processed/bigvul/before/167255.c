void FetchManager::Loader::DidReceiveRedirectTo(const KURL& url) {
  url_list_.push_back(url);
}
