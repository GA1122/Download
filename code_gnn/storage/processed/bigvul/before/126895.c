void BrowserTabStripController::AddTab(WebContents* contents,
                                       int index,
                                       bool is_active) {
  hover_tab_selector_.CancelTabTransition();

  TabRendererData data;
  SetTabRendererDataFromModel(contents, index, &data, NEW_TAB);
  tabstrip_->AddTabAt(index, data, is_active);
}
