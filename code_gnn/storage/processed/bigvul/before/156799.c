LazyLoadImageObserver& Document::EnsureLazyLoadImageObserver() {
  if (!lazy_load_image_observer_)
    lazy_load_image_observer_ = new LazyLoadImageObserver();
  return *lazy_load_image_observer_;
}
