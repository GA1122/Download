bool XSSAuditor::EraseDangerousAttributesIfInjected(
    const FilterTokenRequest& request) {
  bool did_block_script = false;
  for (size_t i = 0; i < request.token.Attributes().size(); ++i) {
    bool erase_attribute = false;
    bool value_contains_java_script_url = false;
    const HTMLToken::Attribute& attribute = request.token.Attributes().at(i);
    if (IsNameOfInlineEventHandler(attribute.NameAsVector())) {
      erase_attribute = IsContainedInRequest(
          Canonicalize(SnippetFromAttribute(request, attribute),
                       kScriptLikeAttributeTruncation));
    } else if (IsSemicolonSeparatedAttribute(attribute)) {
      if (IsSemicolonSeparatedValueContainingJavaScriptURL(attribute.Value())) {
        value_contains_java_script_url = true;
        erase_attribute =
            IsContainedInRequest(Canonicalize(
                NameFromAttribute(request, attribute), kNoTruncation)) &&
            IsContainedInRequest(
                Canonicalize(SnippetFromAttribute(request, attribute),
                             kSemicolonSeparatedScriptLikeAttributeTruncation));
      }
    } else if (ProtocolIsJavaScript(
                   StripLeadingAndTrailingHTMLSpaces(attribute.Value()))) {
      value_contains_java_script_url = true;
      erase_attribute = IsContainedInRequest(
          Canonicalize(SnippetFromAttribute(request, attribute),
                       kScriptLikeAttributeTruncation));
    }
    if (!erase_attribute)
      continue;
    request.token.EraseValueOfAttribute(i);
    if (value_contains_java_script_url)
      request.token.AppendToAttributeValue(i, kSafeJavaScriptURL);
    did_block_script = true;
  }
  return did_block_script;
}
