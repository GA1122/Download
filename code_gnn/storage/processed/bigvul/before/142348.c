void PasswordAccessoryControllerImpl::OnImageFetched(
    url::Origin origin,
    const favicon_base::FaviconRawBitmapResult& bitmap_result) {
  FaviconRequestData* icon_request = &icons_request_data_[origin];

  favicon_base::FaviconImageResult image_result;
  if (bitmap_result.is_valid()) {
    image_result.image = gfx::Image::CreateFrom1xPNGBytes(
        bitmap_result.bitmap_data->front(), bitmap_result.bitmap_data->size());
  }
  icon_request->cached_icon = image_result.image;
  if (origin == current_origin_) {
    for (auto& callback : icon_request->pending_requests) {
      std::move(callback).Run(icon_request->cached_icon);
    }
  }
  icon_request->pending_requests.clear();
}
