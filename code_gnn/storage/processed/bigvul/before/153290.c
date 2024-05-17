void DesktopWindowTreeHostX11::SetWindowIcons(
    const gfx::ImageSkia& window_icon, const gfx::ImageSkia& app_icon) {
  std::vector<unsigned long> data;

  if (!window_icon.isNull())
    SerializeImageRepresentation(window_icon.GetRepresentation(1.0f), &data);

  if (!app_icon.isNull())
    SerializeImageRepresentation(app_icon.GetRepresentation(1.0f), &data);

  if (!data.empty())
    ui::SetAtomArrayProperty(xwindow_, "_NET_WM_ICON", "CARDINAL", data);
}
