base::string16 WebContentsImpl::DumpAccessibilityTree(bool internal) {
  auto* ax_mgr = GetOrCreateRootBrowserAccessibilityManager();
  DCHECK(ax_mgr);
  return AccessibilityTreeFormatter::DumpAccessibilityTreeFromManager(ax_mgr,
                                                                      internal);
}
