bool WebContentsImpl::FocusLocationBarByDefault() {
  NavigationEntry* entry = controller_.GetVisibleEntry();
  if (entry && entry->GetURL() == GURL(kAboutBlankURL))
    return true;
  return delegate_ && delegate_->ShouldFocusLocationBarByDefault(this);
}
