 void FindBarController::EndFindSession(SelectionAction action) {
   find_bar_->Hide(true);
 
    if (tab_contents_) {
    FindManager* find_manager = tab_contents_->GetFindManager();
//     FindTabHelper* find_tab_helper = tab_contents_->find_tab_helper();
  
    find_manager->StopFinding(action);
//     find_tab_helper->StopFinding(action);
  
      if (action != kKeepSelection)
      find_bar_->ClearResults(find_manager->find_result());
//       find_bar_->ClearResults(find_tab_helper->find_result());
  
      find_bar_->RestoreSavedFocus();
   }
 }