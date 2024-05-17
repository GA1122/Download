void BrowserActionsContainer::RefreshBrowserActionViews() {
  for (size_t i = 0; i < browser_action_views_.size(); ++i)
     browser_action_views_[i]->button()->UpdateState();
 }
