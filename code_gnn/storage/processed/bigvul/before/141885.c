void AutofillPopupBaseView::VisibilityChanged(View* starting_from,
                                              bool is_visible) {
  if (is_visible) {
    ui::AXPlatformNode::OnInputSuggestionsAvailable();
    NotifyAccessibilityEvent(ax::mojom::Event::kMenuStart, true);
  } else {
    ui::AXPlatformNode::OnInputSuggestionsUnavailable();
    NotifyAccessibilityEvent(ax::mojom::Event::kMenuEnd, true);
  }
}
