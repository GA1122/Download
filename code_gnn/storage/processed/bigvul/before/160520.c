DownloadUrlParameters::RequestHeadersType WebContentsImpl::ParseDownloadHeaders(
    const std::string& headers) {
  DownloadUrlParameters::RequestHeadersType request_headers;
  for (const base::StringPiece& key_value : base::SplitStringPiece(
           headers, "\r\n", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY)) {
    std::vector<std::string> pair = base::SplitString(
        key_value, ":", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
    if (2ul == pair.size())
      request_headers.push_back(make_pair(pair[0], pair[1]));
  }
  return request_headers;
}
