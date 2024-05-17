bool QPDecode(const std::string& input, std::string* output) {
  std::string temp;
  temp.reserve(input.size());
  for (std::string::const_iterator it = input.begin(); it != input.end();
       ++it) {
    if (*it == '_') {
      temp.push_back(' ');
    } else if (*it == '=') {
      if ((input.end() - it < 3) ||
          !IsHexDigit(static_cast<unsigned char>(*(it + 1))) ||
          !IsHexDigit(static_cast<unsigned char>(*(it + 2))))
        return false;
      unsigned char ch = HexDigitToInt(*(it + 1)) * 16 +
                         HexDigitToInt(*(it + 2));
      temp.push_back(static_cast<char>(ch));
      ++it;
      ++it;
    } else if (0x20 < *it && *it < 0x7F) {
      DCHECK_NE('=', *it);
      DCHECK_NE('?', *it);
      DCHECK_NE('_', *it);
      temp.push_back(*it);
    } else {
      return false;
    }
  }
  output->swap(temp);
  return true;
}
