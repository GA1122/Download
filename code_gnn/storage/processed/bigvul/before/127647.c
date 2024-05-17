void XScopedImage::reset(XImage* image) {
  if (image_ == image)
    return;
  if (image_)
    XDestroyImage(image_);
  image_ = image;
}
