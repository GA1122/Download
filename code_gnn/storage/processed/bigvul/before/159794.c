bool XSSAuditor::EraseAttributeIfInjected(const FilterTokenRequest& request,
                                          const QualifiedName& attribute_name,
                                          const String& replacement_value,
                                          TruncationKind treatment,
                                          HrefRestriction restriction) {
  size_t index_of_attribute = 0;
  if (!FindAttributeWithName(request.token, attribute_name, index_of_attribute))
    return false;

  const HTMLToken::Attribute& attribute =
      request.token.Attributes().at(index_of_attribute);
  if (!IsContainedInRequest(
          Canonicalize(SnippetFromAttribute(request, attribute), treatment)))
    return false;

  if (ThreadSafeMatch(attribute_name, srcAttr) ||
      (restriction == kAllowSameOriginHref &&
       ThreadSafeMatch(attribute_name, hrefAttr))) {
    if (IsLikelySafeResource(attribute.Value()))
      return false;
  } else if (ThreadSafeMatch(attribute_name, http_equivAttr)) {
    if (!IsDangerousHTTPEquiv(attribute.Value()))
      return false;
  }

  request.token.EraseValueOfAttribute(index_of_attribute);
  if (!replacement_value.IsEmpty())
    request.token.AppendToAttributeValue(index_of_attribute, replacement_value);

  return true;
}
