void BrowserTabStripController::TabMoved(TabContents* contents,
                                         int from_model_index,
                                         int to_model_index) {
  hover_tab_selector_.CancelTabTransition();

  TabRendererData data;
  SetTabRendererDataFromModel(contents->web_contents(), to_model_index, &data,
                              EXISTING_TAB);
  tabstrip_->MoveTab(from_model_index, to_model_index, data);
}
