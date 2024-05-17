ui::DropTargetEvent* BrowserRootView::MapEventToTabStrip(
    const ui::DropTargetEvent& event,
    const ui::OSExchangeData& data) {
  gfx::Point tab_strip_loc(event.location());
  ConvertPointToTarget(this, tabstrip(), &tab_strip_loc);
  return new ui::DropTargetEvent(data, tab_strip_loc, tab_strip_loc,
                                 event.source_operations());
}
