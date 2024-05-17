bool HttpUtil::HeadersIterator::GetNext() {
  while (lines_.GetNext()) {
    name_begin_ = lines_.token_begin();
    values_end_ = lines_.token_end();

    string::const_iterator colon = std::find(name_begin_, values_end_, ':');
    if (colon == values_end_)
      continue;   

    name_end_ = colon;

    if (name_begin_ == name_end_ || IsLWS(*name_begin_))
      continue;

    TrimLWS(&name_begin_, &name_end_);
    if (name_begin_ == name_end_)
      continue;   

    values_begin_ = colon + 1;
    TrimLWS(&values_begin_, &values_end_);

    return true;
  }
  return false;
}
