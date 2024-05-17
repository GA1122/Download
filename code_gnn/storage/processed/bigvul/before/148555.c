void WebContentsImpl::SetEncoding(const std::string& encoding) {
  if (encoding == last_reported_encoding_)
    return;
  last_reported_encoding_ = encoding;

  canonical_encoding_ = base::GetCanonicalEncodingNameByAliasName(encoding);
}
