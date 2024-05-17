void CSSStyleSheet::SetAllowRuleAccessFromOrigin(
    scoped_refptr<const SecurityOrigin> allowed_origin) {
  allow_rule_access_from_origin_ = std::move(allowed_origin);
}
