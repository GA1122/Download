void NTPInfoObserver::OnTopSitesLoaded() {
  request_ = top_sites_->StartQueryForMostVisited();
}
