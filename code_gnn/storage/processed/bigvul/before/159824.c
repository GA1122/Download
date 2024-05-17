static bool IsSemicolonSeparatedValueContainingJavaScriptURL(
    const String& value) {
  Vector<String> value_list;
  value.Split(';', value_list);
  for (size_t i = 0; i < value_list.size(); ++i) {
    String stripped = StripLeadingAndTrailingHTMLSpaces(value_list[i]);
    if (ProtocolIsJavaScript(stripped))
      return true;
  }
  return false;
}
