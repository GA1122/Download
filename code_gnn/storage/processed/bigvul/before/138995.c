WallpaperManagerBase::WallpaperManagerBase()
    : loaded_wallpapers_for_test_(0),
      command_line_for_testing_(nullptr),
      should_cache_wallpaper_(false),
      weak_factory_(this) {
  DCHECK(thread_checker_.CalledOnValidThread());
}
