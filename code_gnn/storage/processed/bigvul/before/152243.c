void RenderFrameImpl::DecodeDataURL(const CommonNavigationParams& common_params,
                                    const CommitNavigationParams& commit_params,
                                    std::string* mime_type,
                                    std::string* charset,
                                    std::string* data,
                                    GURL* base_url) {
  GURL data_url = common_params.url;
#if defined(OS_ANDROID)
  if (!commit_params.data_url_as_string.empty()) {
#if DCHECK_IS_ON()
    {
      std::string mime_type_tmp, charset_tmp, data_tmp;
      DCHECK(net::DataURL::Parse(data_url, &mime_type_tmp, &charset_tmp,
                                 &data_tmp));
      DCHECK(data_tmp.empty());
    }
#endif
    data_url = GURL(commit_params.data_url_as_string);
    if (!data_url.is_valid() || !data_url.SchemeIs(url::kDataScheme)) {
      data_url = common_params.url;
    }
  }
#endif
  if (net::DataURL::Parse(data_url, mime_type, charset, data)) {
    *base_url = common_params.base_url_for_data_url.is_empty()
                    ? common_params.url
                    : common_params.base_url_for_data_url;
  } else {
    CHECK(false) << "Invalid URL passed: "
                 << common_params.url.possibly_invalid_spec();
  }
}
