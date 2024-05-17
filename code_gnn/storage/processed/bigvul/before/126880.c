void BrowserView::UpdateDevTools() {
  UpdateDevToolsForContents(GetActiveTabContents());
  Layout();
}
