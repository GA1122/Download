bool XSSAuditor::FilterBaseToken(const FilterTokenRequest& request) {
  DCHECK_EQ(request.token.GetType(), HTMLToken::kStartTag);
  DCHECK(HasName(request.token, baseTag));

  return EraseAttributeIfInjected(request, hrefAttr, String(),
                                  kSrcLikeAttributeTruncation);
}
