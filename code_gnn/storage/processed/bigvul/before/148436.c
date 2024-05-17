bool WebContentsImpl::IsOverridingUserAgent() {
  return GetController().GetVisibleEntry() &&
         GetController().GetVisibleEntry()->GetIsOverridingUserAgent();
}
