bool TopSitesImpl::SetPageThumbnail(const GURL& url,
                                    const gfx::Image& thumbnail,
                                    const ThumbnailScore& score) {
  ThumbnailEvent result = SetPageThumbnailImpl(url, thumbnail, score);

  UMA_HISTOGRAM_ENUMERATION("Thumbnails.AddedToTopSites", result,
                            THUMBNAIL_EVENT_COUNT);

  switch (result) {
    case THUMBNAIL_FAILURE:
    case THUMBNAIL_TOPSITES_FULL:
    case THUMBNAIL_KEPT_EXISTING:
      return false;
    case THUMBNAIL_ADDED_TEMP:
    case THUMBNAIL_ADDED_REGULAR:
      return true;
    case THUMBNAIL_PROMOTED_TEMP_TO_REGULAR:
    case THUMBNAIL_EVENT_COUNT:
      NOTREACHED();
  }

  return false;
}
