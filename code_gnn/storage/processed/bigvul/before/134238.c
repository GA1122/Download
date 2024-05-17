int BrowserRootView::OnDragUpdated(const ui::DropTargetEvent& event) {
  if (ShouldForwardToTabStrip(event)) {
    scoped_ptr<ui::DropTargetEvent> mapped_event(
        MapEventToTabStrip(event, event.data()));
    if (!forwarding_to_tab_strip_) {
      tabstrip()->OnDragEntered(*mapped_event.get());
      forwarding_to_tab_strip_ = true;
    }
    return tabstrip()->OnDragUpdated(*mapped_event.get());
  } else if (forwarding_to_tab_strip_) {
    forwarding_to_tab_strip_ = false;
    tabstrip()->OnDragExited();
  }
  return ui::DragDropTypes::DRAG_NONE;
}
