ui::AXTreeUpdate GetAccessibilityTreeSnapshot(WebContents* web_contents) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents);
  BrowserAccessibilityManager* manager =
      web_contents_impl->GetRootBrowserAccessibilityManager();
  if (!manager)
    return ui::AXTreeUpdate();
  return manager->SnapshotAXTreeForTesting();
}
