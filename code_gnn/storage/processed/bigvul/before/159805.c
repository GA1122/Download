bool XSSAuditor::FilterMetaToken(const FilterTokenRequest& request) {
  DCHECK_EQ(request.token.GetType(), HTMLToken::kStartTag);
  DCHECK(HasName(request.token, metaTag));

  return EraseAttributeIfInjected(request, http_equivAttr);
}
