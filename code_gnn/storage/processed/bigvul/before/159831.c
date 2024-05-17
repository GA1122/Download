static bool StartsHTMLCloseCommentAt(const String& string, size_t start) {
  return (start + 2 < string.length() && string[start] == '-' &&
          string[start + 1] == '-' && string[start + 2] == '>');
}
