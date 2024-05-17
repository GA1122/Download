bool HttpResponseHeaders::GetContentRange(int64* first_byte_position,
                                          int64* last_byte_position,
                                          int64* instance_length) const {
  void* iter = NULL;
  std::string content_range_spec;
  *first_byte_position = *last_byte_position = *instance_length = -1;
  if (!EnumerateHeader(&iter, "content-range", &content_range_spec))
    return false;

  if (content_range_spec.empty())
    return false;

  size_t space_position = content_range_spec.find(' ');
  if (space_position == std::string::npos)
    return false;

  std::string::const_iterator content_range_spec_begin =
      content_range_spec.begin();
  std::string::const_iterator content_range_spec_end =
      content_range_spec.begin() + space_position;
  HttpUtil::TrimLWS(&content_range_spec_begin, &content_range_spec_end);
  if (!LowerCaseEqualsASCII(content_range_spec_begin,
                            content_range_spec_end,
                            "bytes")) {
    return false;
  }

  size_t slash_position = content_range_spec.find('/', space_position + 1);
  if (slash_position == std::string::npos)
    return false;

  std::string::const_iterator byte_range_resp_spec_begin =
      content_range_spec.begin() + space_position + 1;
  std::string::const_iterator byte_range_resp_spec_end =
      content_range_spec.begin() + slash_position;
  HttpUtil::TrimLWS(&byte_range_resp_spec_begin, &byte_range_resp_spec_end);

  std::string byte_range_resp_spec(byte_range_resp_spec_begin,
                                   byte_range_resp_spec_end);
  if (!LowerCaseEqualsASCII(byte_range_resp_spec, "*")) {
    size_t minus_position = byte_range_resp_spec.find('-');
    if (minus_position != std::string::npos) {
      std::string::const_iterator first_byte_pos_begin =
          byte_range_resp_spec.begin();
      std::string::const_iterator first_byte_pos_end =
          byte_range_resp_spec.begin() + minus_position;
      HttpUtil::TrimLWS(&first_byte_pos_begin, &first_byte_pos_end);

      bool ok = base::StringToInt64(first_byte_pos_begin,
                                    first_byte_pos_end,
                                    first_byte_position);

      std::string::const_iterator last_byte_pos_begin =
          byte_range_resp_spec.begin() + minus_position + 1;
      std::string::const_iterator last_byte_pos_end =
          byte_range_resp_spec.end();
      HttpUtil::TrimLWS(&last_byte_pos_begin, &last_byte_pos_end);

      ok &= base::StringToInt64(last_byte_pos_begin,
                                last_byte_pos_end,
                                last_byte_position);
      if (!ok) {
        *first_byte_position = *last_byte_position = -1;
        return false;
      }
      if (*first_byte_position < 0 || *last_byte_position < 0 ||
          *first_byte_position > *last_byte_position)
        return false;
    } else {
      return false;
    }
  }

  std::string::const_iterator instance_length_begin =
      content_range_spec.begin() + slash_position + 1;
  std::string::const_iterator instance_length_end =
      content_range_spec.end();
  HttpUtil::TrimLWS(&instance_length_begin, &instance_length_end);

  if (LowerCaseEqualsASCII(instance_length_begin, instance_length_end, "*")) {
    return false;
  } else if (!base::StringToInt64(instance_length_begin,
                                  instance_length_end,
                                  instance_length)) {
    *instance_length = -1;
    return false;
  }

  if (*first_byte_position < 0 || *last_byte_position < 0 ||
      *instance_length < 0 || *instance_length - 1 < *last_byte_position)
    return false;

  return true;
}
