void HttpResponseHeaders::Update(const HttpResponseHeaders& new_headers) {
  DCHECK(new_headers.response_code() == 304 ||
         new_headers.response_code() == 206);

  std::string new_raw_headers(raw_headers_.c_str());
  new_raw_headers.push_back('\0');

  HeaderSet updated_headers;


  for (size_t i = 0; i < new_headers.parsed_.size(); ++i) {
    const HeaderList& new_parsed = new_headers.parsed_;

    DCHECK(!new_parsed[i].is_continuation());

    size_t k = i;
    while (++k < new_parsed.size() && new_parsed[k].is_continuation()) {}
    --k;

    const std::string::const_iterator& name_begin = new_parsed[i].name_begin;
    const std::string::const_iterator& name_end = new_parsed[i].name_end;
    if (ShouldUpdateHeader(name_begin, name_end)) {
      std::string name(name_begin, name_end);
      StringToLowerASCII(&name);
      updated_headers.insert(name);

      new_raw_headers.append(name_begin, new_parsed[k].value_end);
      new_raw_headers.push_back('\0');
    }

    i = k;
  }

  MergeWithHeaders(new_raw_headers, updated_headers);
}
