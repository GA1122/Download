bool DecodeCharset(const std::string& input,
                   std::string* decoded_charset,
                   std::string* value) {
  StringTokenizer t(input, "'");
  t.set_options(StringTokenizer::RETURN_DELIMS);
  std::string temp_charset;
  std::string temp_value;
  int numDelimsSeen = 0;
  while (t.GetNext()) {
    if (t.token_is_delim()) {
      ++numDelimsSeen;
      continue;
    } else {
      switch (numDelimsSeen) {
        case 0:
          temp_charset = t.token();
          break;
        case 1:
          break;
        case 2:
          temp_value = t.token();
          break;
        default:
          return false;
      }
    }
  }
  if (numDelimsSeen != 2)
    return false;
  if (temp_charset.empty() || temp_value.empty())
    return false;
  decoded_charset->swap(temp_charset);
  value->swap(temp_value);
  return true;
}
