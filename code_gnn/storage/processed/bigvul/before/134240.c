bool BrowserRootView::ShouldForwardToTabStrip(
    const ui::DropTargetEvent& event) {
  if (!tabstrip()->visible())
    return false;

  gfx::Point tab_loc_in_host;
  ConvertPointToTarget(tabstrip(), this, &tab_loc_in_host);
  return event.y() < tab_loc_in_host.y() + tabstrip()->height();
}
