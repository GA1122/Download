void AutofillPopupViewViews::Hide() {
  controller_ = NULL;
  ui::AXPlatformNode::OnInputSuggestionsUnavailable();
  DoHide();
  NotifyAccessibilityEvent(ax::mojom::Event::kMenuEnd, true);
}
