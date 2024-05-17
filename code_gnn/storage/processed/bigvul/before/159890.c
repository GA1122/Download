void TopSitesImpl::OnGotMostVisitedThumbnails(
    const scoped_refptr<MostVisitedThumbnails>& thumbnails) {
  DCHECK(thread_checker_.CalledOnValidThread());

  cache_->SetTopSites(thumbnails->most_visited);
  SetTopSites(thumbnails->most_visited,
              CALL_LOCATION_FROM_ON_GOT_MOST_VISITED_THUMBNAILS);
  cache_->SetThumbnails(thumbnails->url_to_images_map);

  ResetThreadSafeImageCache();

  MoveStateToLoaded();

  timer_.Start(FROM_HERE, kFirstDelayAtStartup, this,
               &TopSitesImpl::StartQueryForMostVisited);
}
