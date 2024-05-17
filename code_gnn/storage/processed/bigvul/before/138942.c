void WallpaperManagerBase::AddObserver(
    WallpaperManagerBase::Observer* observer) {
  observers_.AddObserver(observer);
}
