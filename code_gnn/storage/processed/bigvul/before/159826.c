static bool IsTerminatingCharacter(UChar c) {
  return (c == '&' || c == '/' || c == '"' || c == '\'' || c == '<' ||
          c == '>' || c == ',');
}
