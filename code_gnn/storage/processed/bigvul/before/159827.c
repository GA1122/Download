String XSSAuditor::NameFromAttribute(const FilterTokenRequest& request,
                                     const HTMLToken::Attribute& attribute) {
  int start = attribute.NameRange().start - request.token.StartIndex();
  int end = attribute.ValueRange().start - request.token.StartIndex();
  return request.source_tracker.SourceForToken(request.token)
      .Substring(start, end - start);
}
