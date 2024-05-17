void BrowserView::WillCloseAllTabs(TabStripModel* tab_strip_model) {
  web_contents_close_handler_->WillCloseAllTabs();
}
