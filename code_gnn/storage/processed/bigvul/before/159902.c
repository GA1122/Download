bool TopSitesImpl::SetPageThumbnailEncoded(
    const GURL& url,
    const base::RefCountedMemory* thumbnail,
    const ThumbnailScore& score) {
  DCHECK(cache_->IsKnownURL(url));

  if (!SetPageThumbnailInCache(url, thumbnail, score))
    return false;

  size_t index = cache_->GetURLIndex(url);
  int url_rank = index - cache_->GetNumForcedURLs();
  const MostVisitedURL& most_visited = cache_->top_sites()[index];
  backend_->SetPageThumbnail(most_visited,
                             url_rank < 0 ? -1 : url_rank,
                             *(cache_->GetImage(most_visited.url)));
  return true;
}
