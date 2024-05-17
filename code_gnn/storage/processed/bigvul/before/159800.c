void XSSAuditor::FilterEndToken(const FilterTokenRequest& request) {
  DCHECK(script_tag_nesting_level_);
  state_ = kFilteringTokens;
  if (HasName(request.token, scriptTag)) {
    script_tag_nesting_level_--;
    DCHECK(request.should_allow_cdata || !script_tag_nesting_level_);
  }
}
