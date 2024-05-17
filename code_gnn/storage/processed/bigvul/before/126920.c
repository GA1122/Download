void BrowserTabStripController::PerformDrop(bool drop_before,
                                            int index,
                                            const GURL& url) {
  chrome::NavigateParams params(browser_, url, content::PAGE_TRANSITION_LINK);
  params.tabstrip_index = index;

  if (drop_before) {
    content::RecordAction(UserMetricsAction("Tab_DropURLBetweenTabs"));
    params.disposition = NEW_FOREGROUND_TAB;
  } else {
    content::RecordAction(UserMetricsAction("Tab_DropURLOnTab"));
    params.disposition = CURRENT_TAB;
    params.source_contents = model_->GetTabContentsAt(index);
  }
  params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  chrome::Navigate(&params);
}
