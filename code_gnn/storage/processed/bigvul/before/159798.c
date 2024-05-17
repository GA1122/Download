bool XSSAuditor::FilterCharacterToken(const FilterTokenRequest& request) {
  DCHECK(script_tag_nesting_level_);
  DCHECK_NE(state_, kUninitialized);
  if (state_ == kPermittingAdjacentCharacterTokens)
    return false;

  if (state_ == kFilteringTokens && script_tag_found_in_request_) {
    String snippet = CanonicalizedSnippetForJavaScript(request);
    if (IsContainedInRequest(snippet))
      state_ = kSuppressingAdjacentCharacterTokens;
    else if (!snippet.IsEmpty())
      state_ = kPermittingAdjacentCharacterTokens;
  }
  if (state_ == kSuppressingAdjacentCharacterTokens) {
    request.token.EraseCharacters();
    request.token.AppendToCharacter(' ');
    return true;
  }
  return false;
}
