void ClipboardMessageFilter::OnReadHTML(ui::ClipboardType type,
                                        base::string16* markup,
                                        GURL* url,
                                        uint32* fragment_start,
                                        uint32* fragment_end) {
  std::string src_url_str;
  GetClipboard()->ReadHTML(type, markup, &src_url_str, fragment_start,
                           fragment_end);
  *url = GURL(src_url_str);
}
