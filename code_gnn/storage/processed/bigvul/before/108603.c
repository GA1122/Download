void Shell::PlatformSetTitle(const string16& title) {
  std::string title_utf8 = UTF16ToUTF8(title);
  gtk_window_set_title(GTK_WINDOW(window_), title_utf8.c_str());
}
