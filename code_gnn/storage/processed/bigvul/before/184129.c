  void FindBarController::Show() {
  FindManager* find_manager = tab_contents_->GetFindManager();
//   FindTabHelper* find_tab_helper = tab_contents_->find_tab_helper();
  
  if (!find_manager->find_ui_active()) {
//   if (!find_tab_helper->find_ui_active()) {
      MaybeSetPrepopulateText();
  
    find_manager->set_find_ui_active(true);
//     find_tab_helper->set_find_ui_active(true);
      find_bar_->Show(true);
    }
    find_bar_->SetFocusAndSelection();
 }