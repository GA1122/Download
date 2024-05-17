PageInfoWindowView::PageInfoWindowView(gfx::NativeWindow parent,
                                       Profile* profile,
                                       const GURL& url,
                                       const NavigationEntry::SSLStatus& ssl,
                                       bool show_history)
    : ALLOW_THIS_IN_INITIALIZER_LIST(model_(profile, url, ssl,
                                            show_history, this)),
      cert_id_(ssl.cert_id()) {
  Init(parent);
}
