ui::AXNodeData GetFocusedAccessibilityNodeInfo(WebContents* web_contents) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents);
  BrowserAccessibilityManager* manager =
      web_contents_impl->GetRootBrowserAccessibilityManager();
  if (!manager)
    return ui::AXNodeData();
  BrowserAccessibility* focused_node = manager->GetFocus();
  return focused_node->GetData();
}
