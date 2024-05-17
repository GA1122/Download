 Images* TopSitesCache::GetImage(const GURL& url) {
   return &images_[GetCanonicalURL(url)];
 }
