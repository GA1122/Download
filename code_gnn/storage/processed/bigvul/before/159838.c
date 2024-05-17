static void TruncateForSemicolonSeparatedScriptLikeAttribute(
    String& decoded_snippet) {
  TruncateForScriptLikeAttribute(decoded_snippet);
  size_t position = decoded_snippet.Find(";");
  if (position != kNotFound)
    decoded_snippet.Truncate(position);
}
