bool TopSitesCache::GetPageThumbnail(
    const GURL& url,
    scoped_refptr<base::RefCountedMemory>* bytes) const {
  std::map<GURL, Images>::const_iterator found =
      images_.find(GetCanonicalURL(url));
  if (found != images_.end()) {
    base::RefCountedMemory* data = found->second.thumbnail.get();
    if (data) {
      *bytes = data;
      return true;
    }
  }
  return false;
}
