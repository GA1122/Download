static Vector<String> ParseAcceptAttribute(const String& accept_string,
                                           bool (*predicate)(const String&)) {
  Vector<String> types;
  if (accept_string.IsEmpty())
    return types;

  Vector<String> split_types;
  accept_string.Split(',', false, split_types);
  for (const String& split_type : split_types) {
    String trimmed_type = StripLeadingAndTrailingHTMLSpaces(split_type);
    if (trimmed_type.IsEmpty())
      continue;
    if (!predicate(trimmed_type))
      continue;
    types.push_back(trimmed_type.DeprecatedLower());
  }

  return types;
}
