bool XSSAuditor::FilterFrameToken(const FilterTokenRequest& request) {
  DCHECK_EQ(request.token.GetType(), HTMLToken::kStartTag);
  DCHECK(HasName(request.token, iframeTag) || HasName(request.token, frameTag));

  bool did_block_script = EraseAttributeIfInjected(
      request, srcdocAttr, String(), kScriptLikeAttributeTruncation);
  if (IsContainedInRequest(CanonicalizedSnippetForTagName(request)))
    did_block_script |= EraseAttributeIfInjected(request, srcAttr, String(),
                                                 kSrcLikeAttributeTruncation);

  return did_block_script;
}
