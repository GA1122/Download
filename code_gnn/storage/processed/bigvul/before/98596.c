void DraggedTabGtk::Resize(int width) {
  attached_tab_size_.set_width(width);
  ResizeContainer();
}
