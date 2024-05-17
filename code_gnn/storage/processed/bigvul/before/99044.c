PageInfoWindowGtk::~PageInfoWindowGtk() {
  g_page_info_window_map.erase(url_.spec());
}
