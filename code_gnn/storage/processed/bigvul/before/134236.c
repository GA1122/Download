void BrowserRootView::OnDragEntered(const ui::DropTargetEvent& event) {
  if (ShouldForwardToTabStrip(event)) {
    forwarding_to_tab_strip_ = true;
    scoped_ptr<ui::DropTargetEvent> mapped_event(
        MapEventToTabStrip(event, event.data()));
    tabstrip()->OnDragEntered(*mapped_event.get());
  }
}
