gfx::Rect ImageView::GetImageBounds() const {
  gfx::Size image_size(image_size_set_ ?
    image_size_ : gfx::Size(image_.width(), image_.height()));
  return gfx::Rect(ComputeImageOrigin(image_size), image_size);
}
