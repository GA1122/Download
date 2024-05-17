bool ResourceMultiBufferDataProvider::ParseContentRange(
    const std::string& content_range_str,
    int64_t* first_byte_position,
    int64_t* last_byte_position,
    int64_t* instance_size) {
  const char kUpThroughBytesUnit[] = "bytes ";
  if (!base::StartsWith(content_range_str, kUpThroughBytesUnit,
                        base::CompareCase::SENSITIVE)) {
    return false;
  }
  std::string range_spec =
      content_range_str.substr(sizeof(kUpThroughBytesUnit) - 1);
  size_t dash_offset = range_spec.find("-");
  size_t slash_offset = range_spec.find("/");

  if (dash_offset == std::string::npos || slash_offset == std::string::npos ||
      slash_offset < dash_offset || slash_offset + 1 == range_spec.length()) {
    return false;
  }
  if (!base::StringToInt64(range_spec.substr(0, dash_offset),
                           first_byte_position) ||
      !base::StringToInt64(
          range_spec.substr(dash_offset + 1, slash_offset - dash_offset - 1),
          last_byte_position)) {
    return false;
  }
  if (slash_offset == range_spec.length() - 2 &&
      range_spec[slash_offset + 1] == '*') {
    *instance_size = kPositionNotSpecified;
  } else {
    if (!base::StringToInt64(range_spec.substr(slash_offset + 1),
                             instance_size)) {
      return false;
    }
  }
  if (*last_byte_position < *first_byte_position ||
      (*instance_size != kPositionNotSpecified &&
       *last_byte_position >= *instance_size)) {
    return false;
  }

  return true;
}
