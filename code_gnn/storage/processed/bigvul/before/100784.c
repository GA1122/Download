bool HttpResponseHeaders::EnumerateHeaderLines(void** iter,
                                               std::string* name,
                                               std::string* value) const {
  size_t i = reinterpret_cast<size_t>(*iter);
  if (i == parsed_.size())
    return false;

  DCHECK(!parsed_[i].is_continuation());

  name->assign(parsed_[i].name_begin, parsed_[i].name_end);

  std::string::const_iterator value_begin = parsed_[i].value_begin;
  std::string::const_iterator value_end = parsed_[i].value_end;
  while (++i < parsed_.size() && parsed_[i].is_continuation())
    value_end = parsed_[i].value_end;

  value->assign(value_begin, value_end);

  *iter = reinterpret_cast<void*>(i);
  return true;
}
