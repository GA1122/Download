bool TopSitesImpl::SetPageThumbnailInCache(
    const GURL& url,
    const base::RefCountedMemory* thumbnail_data,
    const ThumbnailScore& score) {
  DCHECK(cache_->IsKnownURL(url));

  const MostVisitedURL& most_visited =
      cache_->top_sites()[cache_->GetURLIndex(url)];
  Images* image = cache_->GetImage(url);

  ThumbnailScore new_score_with_redirects(score);
  new_score_with_redirects.redirect_hops_from_dest =
      GetRedirectDistanceForURL(most_visited, url);

  if (image->thumbnail.get() &&
      !ShouldReplaceThumbnailWith(image->thumbnail_score,
                                  new_score_with_redirects)) {
    return false;   
  }

  image->thumbnail = const_cast<base::RefCountedMemory*>(thumbnail_data);
  image->thumbnail_score = new_score_with_redirects;

  ResetThreadSafeImageCache();
  return true;
}
