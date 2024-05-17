bool HttpUtil::ParseRangeHeader(const std::string& ranges_specifier,
                                std::vector<HttpByteRange>* ranges) {
  size_t equal_char_offset = ranges_specifier.find('=');
  if (equal_char_offset == std::string::npos)
    return false;

  std::string::const_iterator bytes_unit_begin = ranges_specifier.begin();
  std::string::const_iterator bytes_unit_end = bytes_unit_begin +
                                               equal_char_offset;
  std::string::const_iterator byte_range_set_begin = bytes_unit_end + 1;
  std::string::const_iterator byte_range_set_end = ranges_specifier.end();

  TrimLWS(&bytes_unit_begin, &bytes_unit_end);
  if (!LowerCaseEqualsASCII(bytes_unit_begin, bytes_unit_end, "bytes"))
    return false;

  ValuesIterator byte_range_set_iterator(byte_range_set_begin,
                                         byte_range_set_end, ',');
  while (byte_range_set_iterator.GetNext()) {
    size_t minus_char_offset = byte_range_set_iterator.value().find('-');
    if (minus_char_offset == std::string::npos)
      return false;

    std::string::const_iterator first_byte_pos_begin =
        byte_range_set_iterator.value_begin();
    std::string::const_iterator first_byte_pos_end =
        first_byte_pos_begin +  minus_char_offset;
    TrimLWS(&first_byte_pos_begin, &first_byte_pos_end);
    std::string first_byte_pos(first_byte_pos_begin, first_byte_pos_end);

    HttpByteRange range;
    if (!first_byte_pos.empty()) {
      int64 first_byte_position = -1;
      if (!base::StringToInt64(first_byte_pos, &first_byte_position))
        return false;
      range.set_first_byte_position(first_byte_position);
    }

    std::string::const_iterator last_byte_pos_begin =
        byte_range_set_iterator.value_begin() + minus_char_offset + 1;
    std::string::const_iterator last_byte_pos_end =
        byte_range_set_iterator.value_end();
    TrimLWS(&last_byte_pos_begin, &last_byte_pos_end);
    std::string last_byte_pos(last_byte_pos_begin, last_byte_pos_end);

    if (!last_byte_pos.empty()) {
      int64 last_byte_position;
      if (!base::StringToInt64(last_byte_pos, &last_byte_position))
        return false;
      if (range.HasFirstBytePosition())
        range.set_last_byte_position(last_byte_position);
      else
        range.set_suffix_length(last_byte_position);
    } else if (!range.HasFirstBytePosition()) {
      return false;
    }

    if (!range.IsValid())
      return false;
    ranges->push_back(range);
  }
  return !ranges->empty();
}
