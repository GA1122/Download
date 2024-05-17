bool HttpUtil::NameValuePairsIterator::GetNext() {
  if (!props_.GetNext())
    return false;

  value_begin_ = props_.value_begin();
  value_end_ = props_.value_end();
  name_begin_ = name_end_ = value_end_;

  std::string::const_iterator equals = std::find(value_begin_, value_end_, '=');
  if (equals == value_end_ || equals == value_begin_)
    return valid_ = false;   

  for (std::string::const_iterator it = value_begin_; it != equals; ++it) {
    if (HttpUtil::IsQuote(*it))
      return valid_ = false;   
  }

  name_begin_ = value_begin_;
  name_end_ = equals;
  value_begin_ = equals + 1;

  TrimLWS(&name_begin_, &name_end_);
  TrimLWS(&value_begin_, &value_end_);
  value_is_quoted_ = false;
  unquoted_value_.clear();

  if (value_begin_ == value_end_)
    return valid_ = false;   

  if (HttpUtil::IsQuote(*value_begin_)) {
    if (*value_begin_ != *(value_end_ - 1) || value_begin_ + 1 == value_end_) {
      ++value_begin_;   
    } else {
      value_is_quoted_ = true;
      unquoted_value_ = HttpUtil::Unquote(value_begin_, value_end_);
    }
  }

  return true;
}
