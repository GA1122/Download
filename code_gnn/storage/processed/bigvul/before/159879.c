bool TopSitesImpl::GetTemporaryPageThumbnailScore(const GURL& url,
                                                  ThumbnailScore* score) {
  for (const TempImage& temp_image : temp_images_) {
    if (temp_image.first == url) {
      *score = temp_image.second.thumbnail_score;
      return true;
    }
  }
  return false;
}
