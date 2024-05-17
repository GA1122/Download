bool XSSAuditor::FilterFormToken(const FilterTokenRequest& request) {
  DCHECK_EQ(request.token.GetType(), HTMLToken::kStartTag);
  DCHECK(HasName(request.token, formTag));

  return EraseAttributeIfInjected(request, actionAttr, kURLWithUniqueOrigin,
                                  kSrcLikeAttributeTruncation);
}
