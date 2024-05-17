 void UserSelectionScreen::SetHandler(LoginDisplayWebUIHandler* handler) {
   handler_ = handler;
 
  if (handler_) {
    for (user_manager::User* user : users_)
      handler_->OnUserImageChanged(*user);
  }
}
