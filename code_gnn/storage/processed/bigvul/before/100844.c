HttpUtil::NameValuePairsIterator::NameValuePairsIterator(
    string::const_iterator begin,
    string::const_iterator end,
    char delimiter)
    : props_(begin, end, delimiter),
      valid_(true),
      begin_(begin),
      end_(end),
      name_begin_(end),
      name_end_(end),
      value_begin_(end),
      value_end_(end),
      value_is_quoted_(false) {
}
