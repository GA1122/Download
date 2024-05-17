void RenderView::OnSetAccessibilityFocus(int acc_obj_id) {
  if (!accessibility_.get())
    return;

  WebAccessibilityObject obj = accessibility_->getObjectById(acc_obj_id);
  WebAccessibilityObject root = webview()->accessibilityObject();
  if (!obj.isValid() || !root.isValid())
    return;

  if (accessibility_->addOrGetId(obj) == accessibility_->addOrGetId(root))
    webview()->clearFocusedNode();
  else
    obj.setFocused(true);
}
