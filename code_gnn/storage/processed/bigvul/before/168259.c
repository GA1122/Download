base::string16 BrowserView::GetAccessibleWindowTitle() const {
  return GetAccessibleWindowTitleForChannelAndProfile(chrome::GetChannel(),
                                                      browser_->profile());
}
