int BrowserView::GetTabStripHeight() const {
  return IsTabStripVisible() ? tabstrip_->GetPreferredSize().height() : 0;
}
