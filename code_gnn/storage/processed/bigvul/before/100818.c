void HttpResponseHeaders::RemoveHeader(const std::string& name) {
  std::string new_raw_headers(raw_headers_.c_str());
  new_raw_headers.push_back('\0');

  std::string lowercase_name(name);
  StringToLowerASCII(&lowercase_name);
  HeaderSet to_remove;
  to_remove.insert(lowercase_name);
  MergeWithHeaders(new_raw_headers, to_remove);
 }
