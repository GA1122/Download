void RenderFrameImpl::LoadDataURL(const CommonNavigationParams& params,
                                  WebFrame* frame) {
  std::string mime_type, charset, data;
  if (net::DataURL::Parse(params.url, &mime_type, &charset, &data)) {
    const GURL base_url = params.base_url_for_data_url.is_empty() ?
        params.url : params.base_url_for_data_url;
    frame->loadData(
        WebData(data.c_str(), data.length()),
        WebString::fromUTF8(mime_type),
        WebString::fromUTF8(charset),
        base_url,
        params.history_url_for_data_url,
        false);
  } else {
    CHECK(false) << "Invalid URL passed: "
                 << params.url.possibly_invalid_spec();
  }
}
