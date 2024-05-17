bool WallpaperManagerBase::ResizeAndSaveWallpaper(const gfx::ImageSkia& image,
                                                  const base::FilePath& path,
                                                  WallpaperLayout layout,
                                                  int preferred_width,
                                                  int preferred_height,
                                                  gfx::ImageSkia* output_skia) {
  if (layout == WALLPAPER_LAYOUT_CENTER) {
    if (base::PathExists(path))
      base::DeleteFile(path, false);
    return false;
  }
  scoped_refptr<base::RefCountedBytes> data;
  if (ResizeImage(image, layout, preferred_width, preferred_height, &data,
                  output_skia)) {
    return SaveWallpaperInternal(
        path, reinterpret_cast<const char*>(data->front()), data->size());
  }
  return false;
}
