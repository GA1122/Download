static bool IsRequiredForInjection(UChar c) {
  return (c == '\'' || c == '"' || c == '<' || c == '>');
}
