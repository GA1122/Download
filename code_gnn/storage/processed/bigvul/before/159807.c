bool XSSAuditor::FilterParamToken(const FilterTokenRequest& request) {
  DCHECK_EQ(request.token.GetType(), HTMLToken::kStartTag);
  DCHECK(HasName(request.token, paramTag));

  size_t index_of_name_attribute;
  if (!FindAttributeWithName(request.token, nameAttr, index_of_name_attribute))
    return false;

  const HTMLToken::Attribute& name_attribute =
      request.token.Attributes().at(index_of_name_attribute);
  if (!HTMLParamElement::IsURLParameter(name_attribute.Value()))
    return false;

  return EraseAttributeIfInjected(request, valueAttr, BlankURL().GetString(),
                                  kSrcLikeAttributeTruncation);
}
