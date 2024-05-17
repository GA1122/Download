void Extension::SetCachedImage(const ExtensionResource& source,
                               const SkBitmap& image,
                               const gfx::Size& original_size) const {
  DCHECK(source.extension_root() == path());   
  const FilePath& path = source.relative_path();
  gfx::Size actual_size(image.width(), image.height());
  std::string location;
  if (actual_size != original_size)
    location = SizeToString(actual_size);
  image_cache_[ImageCacheKey(path, location)] = image;
}
