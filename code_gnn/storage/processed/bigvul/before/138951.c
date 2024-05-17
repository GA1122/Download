void WallpaperManagerBase::CreateSolidDefaultWallpaper() {
  loaded_wallpapers_for_test_++;
  SkBitmap bitmap;
  bitmap.allocN32Pixels(1, 1);
  bitmap.eraseColor(kDefaultWallpaperColor);
  const gfx::ImageSkia image = gfx::ImageSkia::CreateFrom1xBitmap(bitmap);
   default_wallpaper_image_.reset(new user_manager::UserImage(image));
 }
