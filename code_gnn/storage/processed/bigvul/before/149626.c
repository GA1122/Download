bool ResourcePrefetchPredictor::IsUrlPreconnectable(
    const GURL& main_frame_url) const {
  return PredictPreconnectOrigins(main_frame_url, nullptr);
}
