void PepperVideoRenderer3D::OnViewChanged(const pp::View& view) {
  pp::Size size = view.GetRect().size();
  float scale = view.GetDeviceScale();
  view_size_.set(ceilf(size.width() * scale), ceilf(size.height() * scale));
  graphics_.ResizeBuffers(view_size_.width(), view_size_.height());

  force_repaint_ = true;
   PaintIfNeeded();
 }
