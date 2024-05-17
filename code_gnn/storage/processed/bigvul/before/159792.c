String XSSAuditor::CanonicalizedSnippetForTagName(
    const FilterTokenRequest& request) {
  String source = request.source_tracker.SourceForToken(request.token);

  unsigned start = 0;
  for (start = 0; start < source.length() && source[start] == '\0'; ++start)
    continue;

  return Canonicalize(
      source.Substring(start, request.token.GetName().size() + 1),
      kNoTruncation);
}
