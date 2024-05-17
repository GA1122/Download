void WallpaperManagerBase::RemoveObserver(
    WallpaperManagerBase::Observer* observer) {
  observers_.RemoveObserver(observer);
}
