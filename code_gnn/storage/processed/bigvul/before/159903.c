TopSitesImpl::ThumbnailEvent TopSitesImpl::SetPageThumbnailImpl(
    const GURL& url,
    const gfx::Image& thumbnail,
    const ThumbnailScore& score) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!loaded_) {
    return THUMBNAIL_FAILURE;
  }

  bool add_temp_thumbnail = false;
  if (!IsKnownURL(url)) {
    if (IsNonForcedFull()) {
      return THUMBNAIL_TOPSITES_FULL;
    }

    add_temp_thumbnail = true;
  }

  if (!can_add_url_to_history_.Run(url))
    return THUMBNAIL_FAILURE;   

  scoped_refptr<base::RefCountedBytes> thumbnail_data;
  if (!EncodeBitmap(thumbnail, &thumbnail_data))
    return THUMBNAIL_FAILURE;

  if (add_temp_thumbnail) {
    RemoveTemporaryThumbnailByURL(url);
    AddTemporaryThumbnail(url, thumbnail_data.get(), score);
    return THUMBNAIL_ADDED_TEMP;
  }

  bool success = SetPageThumbnailEncoded(url, thumbnail_data.get(), score);
  return success ? THUMBNAIL_ADDED_REGULAR : THUMBNAIL_KEPT_EXISTING;
}
