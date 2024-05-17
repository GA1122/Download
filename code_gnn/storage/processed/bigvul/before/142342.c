void PasswordAccessoryControllerImpl::GetFavicon(
    int desired_size_in_pixel,
    base::OnceCallback<void(const gfx::Image&)> icon_callback) {
  url::Origin origin = current_origin_;   
  FaviconRequestData* icon_request = &icons_request_data_[origin];
  if (!icon_request->cached_icon.IsEmpty()) {
    std::move(icon_callback).Run(icon_request->cached_icon);
    return;
  }
  if (!favicon_service_) {   
    std::move(icon_callback).Run(gfx::Image());
    return;
  }

  icon_request->pending_requests.emplace_back(std::move(icon_callback));
  if (icon_request->pending_requests.size() > 1)
    return;   

  favicon_service_->GetRawFaviconForPageURL(
      origin.GetURL(),
      {favicon_base::IconType::kFavicon, favicon_base::IconType::kTouchIcon,
       favicon_base::IconType::kTouchPrecomposedIcon,
       favicon_base::IconType::kWebManifestIcon},
      desired_size_in_pixel,
        true,
      base::BindRepeating(   
          &PasswordAccessoryControllerImpl::OnImageFetched,
          base::AsWeakPtr<PasswordAccessoryControllerImpl>(this), origin),
      &favicon_tracker_);
}
