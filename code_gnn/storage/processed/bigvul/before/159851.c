bool TopSitesCache::GetPageThumbnailScore(const GURL& url,
                                          ThumbnailScore* score) const {
  std::map<GURL, Images>::const_iterator found =
      images_.find(GetCanonicalURL(url));
  if (found != images_.end()) {
    *score = found->second.thumbnail_score;
    return true;
  }
  return false;
}
