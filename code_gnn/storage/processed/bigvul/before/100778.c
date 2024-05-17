void HttpResponseHeaders::AddHeader(std::string::const_iterator name_begin,
                                    std::string::const_iterator name_end,
                                    std::string::const_iterator values_begin,
                                    std::string::const_iterator values_end) {
  if (values_begin == values_end ||
      HttpUtil::IsNonCoalescingHeader(name_begin, name_end)) {
    AddToParsed(name_begin, name_end, values_begin, values_end);
  } else {
    HttpUtil::ValuesIterator it(values_begin, values_end, ',');
    while (it.GetNext()) {
      AddToParsed(name_begin, name_end, it.value_begin(), it.value_end());
      name_begin = name_end = raw_headers_.end();
    }
  }
}
