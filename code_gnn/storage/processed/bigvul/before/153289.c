void DesktopWindowTreeHostX11::SetWMSpecState(bool enabled,
                                              XAtom state1,
                                              XAtom state2) {
  if (window_mapped_in_client_) {
    ui::SetWMSpecState(xwindow_, enabled, state1, state2);
  } else {
    base::flat_set<XAtom> new_window_properties = window_properties_;
    for (XAtom atom : {state1, state2}) {
      if (enabled)
        new_window_properties.insert(atom);
      else
        new_window_properties.erase(atom);
    }
    UpdateWindowProperties(new_window_properties);
  }
}
