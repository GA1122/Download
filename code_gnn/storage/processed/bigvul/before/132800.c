bool PictureLayer::HasDrawableContent() const {
  return client_ && Layer::HasDrawableContent();
}
