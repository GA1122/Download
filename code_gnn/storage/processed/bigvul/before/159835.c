static bool StartsSingleLineCommentAt(const String& string, size_t start) {
  return (start + 1 < string.length() && string[start] == '/' &&
          string[start + 1] == '/');
}
