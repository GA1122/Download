int BrowserActionsContainer::GetCurrentTabId() const {
  TabContents* tab_contents = toolbar_->browser()->GetSelectedTabContents();
  if (!tab_contents)
    return -1;

  return tab_contents->controller().session_id().id();
}
