void AutofillPopupViewViews::Show() {
  DoShow();
  ui::AXPlatformNode::OnInputSuggestionsAvailable();
  NotifyAccessibilityEvent(ax::mojom::Event::kMenuStart, true);
}
