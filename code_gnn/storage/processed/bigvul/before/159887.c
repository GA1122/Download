bool TopSitesImpl::IsNonForcedFull() {
  return loaded_ && cache_->GetNumNonForcedURLs() >= kNonForcedTopSitesNumber;
}
