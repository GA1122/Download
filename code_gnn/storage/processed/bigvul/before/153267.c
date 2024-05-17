void DesktopWindowTreeHostX11::OnWMStateUpdated() {
  std::vector<XAtom> atom_list;
  if (ui::GetAtomArrayProperty(xwindow_, "_NET_WM_STATE", &atom_list) ||
      window_mapped_in_client_) {
    UpdateWindowProperties(
        base::flat_set<XAtom>(std::begin(atom_list), std::end(atom_list)));
  }
}
