void BrowserTabStripController::OnDropIndexUpdate(int index,
                                                  bool drop_before) {
  if (index != -1 && !drop_before) {
    hover_tab_selector_.StartTabTransition(index);
  } else {
    hover_tab_selector_.CancelTabTransition();
  }
}
