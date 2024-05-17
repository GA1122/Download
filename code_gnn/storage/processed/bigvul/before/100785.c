size_t HttpResponseHeaders::FindHeader(size_t from,
                                       const std::string& search) const {
  for (size_t i = from; i < parsed_.size(); ++i) {
    if (parsed_[i].is_continuation())
      continue;
    const std::string::const_iterator& name_begin = parsed_[i].name_begin;
    const std::string::const_iterator& name_end = parsed_[i].name_end;
    if (static_cast<size_t>(name_end - name_begin) == search.size() &&
        std::equal(name_begin, name_end, search.begin(),
                   base::CaseInsensitiveCompare<char>()))
      return i;
  }

  return std::string::npos;
}
