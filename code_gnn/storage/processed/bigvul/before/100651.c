bool CompareURLsWithReplacements(
    const GURL& url,
    const GURL& other,
    const url_canon::Replacements<char>& replacements) {
  if (url == other)
    return true;

  GURL url_replaced = url.ReplaceComponents(replacements);
  GURL other_replaced = other.ReplaceComponents(replacements);
  return url_replaced == other_replaced;
}
