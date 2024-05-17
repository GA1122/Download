void TopSitesImpl::SyncWithHistory() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (loaded_)
    StartQueryForMostVisited();
}
