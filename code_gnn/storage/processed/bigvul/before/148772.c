AccessibilityMode InterstitialPageImpl::GetAccessibilityMode() const {
  if (web_contents_)
    return static_cast<WebContentsImpl*>(web_contents_)->GetAccessibilityMode();
  else
    return AccessibilityMode();
}
