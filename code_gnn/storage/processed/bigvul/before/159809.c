bool XSSAuditor::FilterStartToken(const FilterTokenRequest& request) {
  state_ = kFilteringTokens;
  bool did_block_script = EraseDangerousAttributesIfInjected(request);

  if (HasName(request.token, scriptTag)) {
    did_block_script |= FilterScriptToken(request);
    DCHECK(request.should_allow_cdata || !script_tag_nesting_level_);
    script_tag_nesting_level_++;
  } else if (HasName(request.token, objectTag))
    did_block_script |= FilterObjectToken(request);
  else if (HasName(request.token, paramTag))
    did_block_script |= FilterParamToken(request);
  else if (HasName(request.token, embedTag))
    did_block_script |= FilterEmbedToken(request);
  else if (HasName(request.token, iframeTag) ||
           HasName(request.token, frameTag))
    did_block_script |= FilterFrameToken(request);
  else if (HasName(request.token, metaTag))
    did_block_script |= FilterMetaToken(request);
  else if (HasName(request.token, baseTag))
    did_block_script |= FilterBaseToken(request);
  else if (HasName(request.token, formTag))
    did_block_script |= FilterFormToken(request);
  else if (HasName(request.token, inputTag))
    did_block_script |= FilterInputToken(request);
  else if (HasName(request.token, buttonTag))
    did_block_script |= FilterButtonToken(request);
  else if (HasName(request.token, linkTag))
    did_block_script |= FilterLinkToken(request);

  return did_block_script;
}
