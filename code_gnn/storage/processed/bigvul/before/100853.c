HttpUtil::ValuesIterator::ValuesIterator(
    string::const_iterator values_begin,
    string::const_iterator values_end,
    char delimiter)
    : values_(values_begin, values_end, string(1, delimiter)) {
  values_.set_quote_chars("\'\"");
}
