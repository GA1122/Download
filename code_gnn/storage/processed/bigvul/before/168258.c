base::string16 BrowserView::GetAccessibleTabLabel(bool include_app_name,
                                                  int index) const {
  if (index == -1)
    return base::string16();

  base::string16 window_title =
      browser_->GetWindowTitleForTab(include_app_name, index);
  return chrome::AssembleTabAccessibilityLabel(
      window_title, tabstrip_->IsTabCrashed(index),
      tabstrip_->TabHasNetworkError(index), tabstrip_->GetTabAlertState(index));
}
