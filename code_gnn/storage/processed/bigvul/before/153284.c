void DesktopWindowTreeHostX11::SetOpacity(float opacity) {

  unsigned long opacity_8bit =
      static_cast<unsigned long>(opacity * 255.0f) & 0xFF;
  const unsigned long channel_multiplier = 0x1010101;
  unsigned long cardinality = opacity_8bit * channel_multiplier;

  if (cardinality == 0xffffffff) {
    XDeleteProperty(xdisplay_, xwindow_,
                    gfx::GetAtom("_NET_WM_WINDOW_OPACITY"));
  } else {
    XChangeProperty(xdisplay_, xwindow_, gfx::GetAtom("_NET_WM_WINDOW_OPACITY"),
                    XA_CARDINAL, 32, PropModeReplace,
                    reinterpret_cast<unsigned char*>(&cardinality), 1);
  }
}
