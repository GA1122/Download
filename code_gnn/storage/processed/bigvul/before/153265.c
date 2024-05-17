void DesktopWindowTreeHostX11::OnFrameExtentsUpdated() {
  std::vector<int> insets;
  if (ui::GetIntArrayProperty(xwindow_, "_NET_FRAME_EXTENTS", &insets) &&
      insets.size() == 4) {
    native_window_frame_borders_in_pixels_ =
        gfx::Insets(insets[2], insets[0], insets[3], insets[1]);
  } else {
    native_window_frame_borders_in_pixels_ = gfx::Insets();
  }
}
