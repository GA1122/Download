bool UnloadController::TabsNeedBeforeUnloadFired() {
  if (tabs_needing_before_unload_fired_.empty()) {
    for (int i = 0; i < browser_->tab_count(); ++i) {
      content::WebContents* contents =
          chrome::GetTabContentsAt(browser_, i)->web_contents();
      if (contents->NeedToFireBeforeUnload())
        tabs_needing_before_unload_fired_.insert(contents);
    }
  }
  return !tabs_needing_before_unload_fired_.empty();
}
