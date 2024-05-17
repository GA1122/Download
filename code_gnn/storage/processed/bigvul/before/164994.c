FloatSize HTMLCanvasElement::ElementSize(const FloatSize&) const {
  if (context_ && HasImageBitmapContext()) {
    scoped_refptr<Image> image = context_->GetImage(kPreferNoAcceleration);
    if (image)
      return FloatSize(image->width(), image->height());
    return FloatSize(0, 0);
  }
  if (PlaceholderFrame())
    return FloatSize(PlaceholderFrame()->Size());
  return FloatSize(width(), height());
}
