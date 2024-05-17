bool HttpResponseHeaders::EnumerateHeader(void** iter, const std::string& name,
                                          std::string* value) const {
  size_t i;
  if (!iter || !*iter) {
    i = FindHeader(0, name);
  } else {
    i = reinterpret_cast<size_t>(*iter);
    if (i >= parsed_.size()) {
      i = std::string::npos;
    } else if (!parsed_[i].is_continuation()) {
      i = FindHeader(i, name);
    }
  }

  if (i == std::string::npos) {
    value->clear();
    return false;
  }

  if (iter)
    *iter = reinterpret_cast<void*>(i + 1);
  value->assign(parsed_[i].value_begin, parsed_[i].value_end);
  return true;
}
