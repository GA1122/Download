void HttpResponseHeaders::Persist(Pickle* pickle, PersistOptions options) {
  if (options == PERSIST_RAW) {
    pickle->WriteString(raw_headers_);
    return;   
  }

  HeaderSet filter_headers;

  if ((options & PERSIST_SANS_NON_CACHEABLE) == PERSIST_SANS_NON_CACHEABLE)
    AddNonCacheableHeaders(&filter_headers);

  if ((options & PERSIST_SANS_COOKIES) == PERSIST_SANS_COOKIES)
    AddCookieHeaders(&filter_headers);

  if ((options & PERSIST_SANS_CHALLENGES) == PERSIST_SANS_CHALLENGES)
    AddChallengeHeaders(&filter_headers);

  if ((options & PERSIST_SANS_HOP_BY_HOP) == PERSIST_SANS_HOP_BY_HOP)
    AddHopByHopHeaders(&filter_headers);

  if ((options & PERSIST_SANS_RANGES) == PERSIST_SANS_RANGES)
    AddHopContentRangeHeaders(&filter_headers);

  std::string blob;
  blob.reserve(raw_headers_.size());

  blob.assign(raw_headers_.c_str(), strlen(raw_headers_.c_str()) + 1);

  for (size_t i = 0; i < parsed_.size(); ++i) {
    DCHECK(!parsed_[i].is_continuation());

    size_t k = i;
    while (++k < parsed_.size() && parsed_[k].is_continuation()) {}
    --k;

    std::string header_name(parsed_[i].name_begin, parsed_[i].name_end);
    StringToLowerASCII(&header_name);

    if (filter_headers.find(header_name) == filter_headers.end()) {
      blob.append(parsed_[i].name_begin, parsed_[k].value_end);
      blob.push_back('\0');
    }

    i = k;
  }
  blob.push_back('\0');

  pickle->WriteString(blob);
}
