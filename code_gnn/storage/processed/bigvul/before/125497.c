 std::string GDataEntry::EscapeUtf8FileName(const std::string& input) {
   std::string output;
  if (ReplaceChars(input, kSlash, std::string(kEscapedSlash), &output))
    return output;

  return input;
}
