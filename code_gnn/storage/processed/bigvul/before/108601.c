void Shell::PlatformSetAddressBarURL(const GURL& url) {
  gtk_entry_set_text(GTK_ENTRY(url_edit_view_), url.spec().c_str());
}
