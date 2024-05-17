void ImageBitmapFactories::AddLoader(ImageBitmapLoader* loader) {
  pending_loaders_.insert(loader);
}
