void DesktopWindowTreeHostX11::Maximize() {
  if (ui::HasWMSpecProperty(window_properties_,
                            gfx::GetAtom("_NET_WM_STATE_FULLSCREEN"))) {
    SetWMSpecState(false, gfx::GetAtom("_NET_WM_STATE_FULLSCREEN"), x11::None);

    gfx::Rect adjusted_bounds_in_pixels(bounds_in_pixels_.origin(),
                                        AdjustSize(bounds_in_pixels_.size()));
    if (adjusted_bounds_in_pixels != bounds_in_pixels_)
      SetBoundsInPixels(adjusted_bounds_in_pixels);
  }

  should_maximize_after_map_ = !window_mapped_in_client_;

  restored_bounds_in_pixels_ = bounds_in_pixels_;

  SetWMSpecState(true, gfx::GetAtom("_NET_WM_STATE_MAXIMIZED_VERT"),
                 gfx::GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ"));
  if (IsMinimized())
    Show(ui::SHOW_STATE_NORMAL, gfx::Rect());
}
