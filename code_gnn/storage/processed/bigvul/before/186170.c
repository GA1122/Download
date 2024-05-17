  void UserSelectionScreen::OnBeforeUserRemoved(const AccountId& account_id) {
  for (user_manager::UserList::iterator it = users_.begin(); it != users_.end();
       ++it) {
//   for (auto it = users_.cbegin(); it != users_.cend(); ++it) {
      if ((*it)->GetAccountId() == account_id) {
        users_.erase(it);
        break;
     }
   }
 }