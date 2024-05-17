static String FullyDecodeString(const String& string,
                                const WTF::TextEncoding& encoding) {
  size_t old_working_string_length;
  String working_string = string;
  do {
    old_working_string_length = working_string.length();
    working_string = Decode16BitUnicodeEscapeSequences(
        DecodeStandardURLEscapeSequences(working_string, encoding));
  } while (working_string.length() < old_working_string_length);
  working_string.Replace('+', ' ');
  return working_string;
}
