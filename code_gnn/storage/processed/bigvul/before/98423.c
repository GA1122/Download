gfx::Size ImageView::GetPreferredSize() {
  gfx::Insets insets = GetInsets();
  if (image_size_set_) {
    gfx::Size image_size;
    GetImageSize(&image_size);
    image_size.Enlarge(insets.width(), insets.height());
    return image_size;
  }
  return gfx::Size(image_.width() + insets.width(),
                   image_.height() + insets.height());
}
