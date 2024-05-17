void HttpResponseHeaders::MergeWithHeaders(const std::string& raw_headers,
                                           const HeaderSet& headers_to_remove) {
  std::string new_raw_headers(raw_headers);
  for (size_t i = 0; i < parsed_.size(); ++i) {
    DCHECK(!parsed_[i].is_continuation());

    size_t k = i;
    while (++k < parsed_.size() && parsed_[k].is_continuation()) {}
    --k;

    std::string name(parsed_[i].name_begin, parsed_[i].name_end);
    StringToLowerASCII(&name);
    if (headers_to_remove.find(name) == headers_to_remove.end()) {
      new_raw_headers.append(parsed_[i].name_begin, parsed_[k].value_end);
      new_raw_headers.push_back('\0');
    }

    i = k;
  }
  new_raw_headers.push_back('\0');

  raw_headers_.clear();
  parsed_.clear();
  Parse(new_raw_headers);
}
