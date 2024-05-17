String AXObject::collapseWhitespace(const String& str) {
  StringBuilder result;
  if (!str.isEmpty() && isHTMLSpace<UChar>(str[0]))
    result.append(' ');
  result.append(str.simplifyWhiteSpace(isHTMLSpace<UChar>));
  if (!str.isEmpty() && isHTMLSpace<UChar>(str[str.length() - 1]))
    result.append(' ');
  return result.toString();
}
