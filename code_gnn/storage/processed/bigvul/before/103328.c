void ClipboardMessageFilter::OnReadHTML(
    ui::Clipboard::Buffer buffer, string16* markup, GURL* url) {
  std::string src_url_str;
  GetClipboard()->ReadHTML(buffer, markup, &src_url_str);
  *url = GURL(src_url_str);
}
