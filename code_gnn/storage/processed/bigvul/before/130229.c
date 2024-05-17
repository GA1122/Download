void ClipboardUtil::CFHtmlExtractMetadata(const std::string& cf_html,
                                          std::string* base_url,
                                          size_t* html_start,
                                          size_t* fragment_start,
                                          size_t* fragment_end) {
  if (base_url) {
    static std::string src_url_str("SourceURL:");
    size_t line_start = cf_html.find(src_url_str);
    if (line_start != std::string::npos) {
      size_t src_end = cf_html.find("\n", line_start);
      size_t src_start = line_start + src_url_str.length();
      if (src_end != std::string::npos && src_start != std::string::npos) {
        *base_url = cf_html.substr(src_start, src_end - src_start);
        base::TrimWhitespace(*base_url, base::TRIM_ALL, base_url);
      }
    }
  }

  std::string cf_html_lower = StringToLowerASCII(cf_html);
  size_t markup_start = cf_html_lower.find("<html", 0);
  if (html_start) {
    *html_start = markup_start;
  }
  size_t tag_start = cf_html.find("<!--StartFragment", markup_start);
  if (tag_start == std::string::npos) {
    static std::string start_fragment_str("StartFragment:");
    size_t start_fragment_start = cf_html.find(start_fragment_str);
    if (start_fragment_start != std::string::npos) {
      *fragment_start = static_cast<size_t>(atoi(cf_html.c_str() +
          start_fragment_start + start_fragment_str.length()));
    }

    static std::string end_fragment_str("EndFragment:");
    size_t end_fragment_start = cf_html.find(end_fragment_str);
    if (end_fragment_start != std::string::npos) {
      *fragment_end = static_cast<size_t>(atoi(cf_html.c_str() +
          end_fragment_start + end_fragment_str.length()));
    }
  } else {
    *fragment_start = cf_html.find('>', tag_start) + 1;
    size_t tag_end = cf_html.rfind("<!--EndFragment", std::string::npos);
    *fragment_end = cf_html.rfind('<', tag_end);
  }
}
