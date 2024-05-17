void DesktopWindowTreeHostX11::SetWindowTransparency() {
  compositor()->SetBackgroundColor(use_argb_visual_ ? SK_ColorTRANSPARENT
                                                    : SK_ColorWHITE);
  window()->SetTransparent(use_argb_visual_);
  content_window()->SetTransparent(use_argb_visual_);
}
