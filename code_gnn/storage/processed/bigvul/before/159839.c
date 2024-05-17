static void TruncateForSrcLikeAttribute(String& decoded_snippet) {
  int slash_count = 0;
  bool comma_seen = false;
  bool colon_seen = false;
  for (size_t current_length = 0, remaining_length = decoded_snippet.length();
       remaining_length; ++current_length, --remaining_length) {
    UChar current_char = decoded_snippet[current_length];
    if (current_char == ':' && !colon_seen) {
      if (remaining_length > 1 && !IsSlash(decoded_snippet[current_length + 1]))
        ++slash_count;
      if (remaining_length > 2 && !IsSlash(decoded_snippet[current_length + 2]))
        ++slash_count;
      colon_seen = true;
    }
    if (current_char == '&' || current_char == '?' || current_char == '#' ||
        (IsSlash(current_char) && (comma_seen || ++slash_count > 2)) ||
        (current_char == '<' && comma_seen) ||
        (current_char == '\'' && comma_seen) ||
        (current_char == '"' && comma_seen)) {
      decoded_snippet.Truncate(current_length);
      return;
    }
    if (current_char == ',')
      comma_seen = true;
  }
}
