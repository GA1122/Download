HTMLImageLoader& HTMLInputElement::EnsureImageLoader() {
  if (!image_loader_)
    image_loader_ = HTMLImageLoader::Create(this);
  return *image_loader_;
}
