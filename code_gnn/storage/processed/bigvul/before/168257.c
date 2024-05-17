void BrowserView::GetAccessiblePanes(std::vector<views::View*>* panes) {
  panes->push_back(toolbar_button_provider_->GetAsAccessiblePaneView());
  if (bookmark_bar_view_.get())
    panes->push_back(bookmark_bar_view_.get());
  if (infobar_container_)
    panes->push_back(infobar_container_);
  if (download_shelf_.get())
    panes->push_back(download_shelf_.get());
  panes->push_back(contents_web_view_);
  if (devtools_web_view_->visible())
    panes->push_back(devtools_web_view_);
}
