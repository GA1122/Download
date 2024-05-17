base::string16 Elide(const GURL& url,
                     const gfx::FontList& font_list,
                     float available_width,
                     ElisionMethod method) {
  switch (method) {
    case kMethodSimple: {
      url::Parsed parsed;
      return url_formatter::ElideUrlSimple(url, font_list, available_width,
                                           &parsed);
    }
#if !defined(OS_ANDROID)
    case kMethodOriginal:
      return url_formatter::ElideUrl(url, font_list, available_width);
#endif
    default:
      NOTREACHED();
      return base::string16();
  }
}
