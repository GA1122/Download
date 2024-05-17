void ShowPageInfo(gfx::NativeWindow parent,
                  Profile* profile,
                  const GURL& url,
                  const NavigationEntry::SSLStatus& ssl,
                  bool show_history) {
  PageInfoWindowMap::iterator iter =
      g_page_info_window_map.find(url.spec());
  if (iter != g_page_info_window_map.end()) {
    gtk_window_present(GTK_WINDOW(iter->second->widget()));
    return;
  }

  PageInfoWindowGtk* page_info_window =
      new PageInfoWindowGtk(parent, profile, url, ssl, show_history);
  page_info_window->Show();
}
