void ShowPageInfo(gfx::NativeWindow parent,
                  Profile* profile,
                  const GURL& url,
                  const NavigationEntry::SSLStatus& ssl,
                  bool show_history) {
  PageInfoWindowView* page_info_window =
      new PageInfoWindowView(parent, profile, url, ssl, show_history);
  page_info_window->Show();
}
