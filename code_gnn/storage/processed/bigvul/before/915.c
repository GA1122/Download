void CairoImage::setImage (cairo_surface_t *image) {
  if (this->image)
    cairo_surface_destroy (this->image);
  this->image = cairo_surface_reference (image);
}
