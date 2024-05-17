bool XSSAuditor::FilterEmbedToken(const FilterTokenRequest& request) {
  DCHECK_EQ(request.token.GetType(), HTMLToken::kStartTag);
  DCHECK(HasName(request.token, embedTag));

  bool did_block_script = false;
  if (IsContainedInRequest(CanonicalizedSnippetForTagName(request))) {
    did_block_script |= EraseAttributeIfInjected(request, codeAttr, String(),
                                                 kSrcLikeAttributeTruncation);
    did_block_script |= EraseAttributeIfInjected(
        request, srcAttr, BlankURL().GetString(), kSrcLikeAttributeTruncation);
    did_block_script |= EraseAttributeIfInjected(request, typeAttr);
  }
  return did_block_script;
}
