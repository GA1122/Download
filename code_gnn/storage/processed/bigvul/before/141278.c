void Document::UpdateBaseURL() {
  KURL old_base_url = base_url_;
  if (!base_element_url_.IsEmpty())
    base_url_ = base_element_url_;
  else if (!base_url_override_.IsEmpty())
    base_url_ = base_url_override_;
  else
    base_url_ = FallbackBaseURL();

  GetSelectorQueryCache().Invalidate();

  if (!base_url_.IsValid())
    base_url_ = KURL();

  if (elem_sheet_) {
    DCHECK(!elem_sheet_->Contents()->RuleCount());
    elem_sheet_ = CSSStyleSheet::CreateInline(*this, base_url_);
  }

  if (!EqualIgnoringFragmentIdentifier(old_base_url, base_url_)) {
    for (HTMLAnchorElement& anchor :
         Traversal<HTMLAnchorElement>::StartsAfter(*this))
      anchor.InvalidateCachedVisitedLinkHash();
  }
}
