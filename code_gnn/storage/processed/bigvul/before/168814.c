void CSSPaintValue::PaintImageGeneratorReady() {
  for (const ImageResourceObserver* client : Clients().Keys()) {
    const_cast<ImageResourceObserver*>(client)->ImageChanged(
        static_cast<WrappedImagePtr>(this),
        ImageResourceObserver::CanDeferInvalidation::kNo);
  }
}
