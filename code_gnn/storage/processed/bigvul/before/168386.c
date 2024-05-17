void BrowserView::ViewHierarchyChanged(
    const ViewHierarchyChangedDetails& details) {
  if (details.child != this)
    return;

  auto* widget = details.is_add ? GetWidget() : details.parent->GetWidget();
  if (!widget)
    return;

  bool init = !initialized_ && details.is_add;
  if (init) {
    InitViews();
    initialized_ = true;
  }
}
