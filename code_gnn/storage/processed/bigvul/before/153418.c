void TabStrip::HandleDragUpdate(
    const base::Optional<BrowserRootView::DropIndex>& index) {
  SetDropArrow(index);
}
