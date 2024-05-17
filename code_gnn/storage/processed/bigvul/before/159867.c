void TopSitesImpl::AddTemporaryThumbnail(const GURL& url,
                                         base::RefCountedMemory* thumbnail,
                                         const ThumbnailScore& score) {
  if (temp_images_.size() == kMaxTempTopImages)
    temp_images_.pop_front();

  TempImage image;
  image.first = url;
  image.second.thumbnail = thumbnail;
  image.second.thumbnail_score = score;
  temp_images_.push_back(image);
}
