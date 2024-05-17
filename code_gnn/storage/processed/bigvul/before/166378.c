GLSLArrayName::GLSLArrayName(const std::string& name) : element_index_(-1) {
  if (name.size() < 4)
    return;
  if (name.back() != ']')
    return;

  size_t open_pos = name.find_last_of('[');
  if (open_pos >= name.size() - 2)
    return;

  base::CheckedNumeric<int> index = 0;
  size_t last = name.size() - 1;
  for (size_t pos = open_pos + 1; pos < last; ++pos) {
    int8_t digit = name[pos] - '0';
    if (digit < 0 || digit > 9)
      return;
    index = index * 10 + digit;
  }
  if (!index.IsValid())
    return;
  element_index_ = index.ValueOrDie();
  base_name_ = name.substr(0, open_pos);
}
