void WallpaperManager::OnPolicyFetched(const std::string& policy,
                                       const AccountId& account_id,
                                       std::unique_ptr<std::string> data) {
  if (!data)
    return;

  user_image_loader::StartWithData(
      task_runner_, std::move(data), ImageDecoder::ROBUST_JPEG_CODEC,
      0,   
      base::Bind(&WallpaperManager::SetPolicyControlledWallpaper,
                 weak_factory_.GetWeakPtr(), account_id));
}
