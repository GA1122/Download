void WallpaperManagerBase::OnCustomizedDefaultWallpaperDecoded(
    const GURL& wallpaper_url,
    std::unique_ptr<CustomizedWallpaperRescaledFiles> rescaled_files,
    std::unique_ptr<user_manager::UserImage> wallpaper) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (wallpaper->image().isNull()) {
    LOG(WARNING) << "Failed to decode customized wallpaper.";
    return;
  }

  wallpaper->image().EnsureRepsForSupportedScales();
  std::unique_ptr<gfx::ImageSkia> deep_copy(wallpaper->image().DeepCopy());

  std::unique_ptr<bool> success(new bool(false));
  std::unique_ptr<gfx::ImageSkia> small_wallpaper_image(new gfx::ImageSkia);
  std::unique_ptr<gfx::ImageSkia> large_wallpaper_image(new gfx::ImageSkia);

  base::Closure resize_closure = base::Bind(
      &WallpaperManagerBase::ResizeCustomizedDefaultWallpaper,
      base::Passed(&deep_copy),
      base::Unretained(rescaled_files.get()), base::Unretained(success.get()),
      base::Unretained(small_wallpaper_image.get()),
      base::Unretained(large_wallpaper_image.get()));
  base::Closure on_resized_closure = base::Bind(
      &WallpaperManagerBase::OnCustomizedDefaultWallpaperResized,
      weak_factory_.GetWeakPtr(), wallpaper_url,
      base::Passed(std::move(rescaled_files)), base::Passed(std::move(success)),
      base::Passed(std::move(small_wallpaper_image)),
      base::Passed(std::move(large_wallpaper_image)));

  if (!task_runner_->PostTaskAndReply(FROM_HERE, resize_closure,
                                      on_resized_closure)) {
    LOG(WARNING) << "Failed to start Customized Wallpaper resize.";
  }
}
