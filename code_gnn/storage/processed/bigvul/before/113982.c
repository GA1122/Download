ash::NetworkIconInfo CreateNetworkIconInfo(const Network* network,
                                           NetworkMenuIcon* network_icon,
                                           NetworkMenu* network_menu) {
  ash::NetworkIconInfo info;
  info.name = UTF8ToUTF16(network->name());
  info.image = network_icon->GetBitmap(network, NetworkMenuIcon::COLOR_DARK);
  info.service_path = network->service_path();
  info.highlight = network_menu->ShouldHighlightNetwork(network);
  info.tray_icon_visible = ShouldShowNetworkIconInTray(network);
  return info;
}
