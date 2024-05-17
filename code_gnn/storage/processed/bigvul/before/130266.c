static void GetInternetShortcutFileContents(const GURL& url,
                                            std::string* data) {
  DCHECK(data);
  static const std::string kInternetShortcutFileStart =
      "[InternetShortcut]\r\nURL=";
  static const std::string kInternetShortcutFileEnd =
      "\r\n";
  *data = kInternetShortcutFileStart + url.spec() + kInternetShortcutFileEnd;
}
