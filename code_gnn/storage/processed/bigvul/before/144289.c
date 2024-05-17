void LoginDisplayHostWebUI::CreateExistingUserController() {
  existing_user_controller_.reset();
  existing_user_controller_.reset(new ExistingUserController(this));
  login_display_->set_delegate(existing_user_controller_.get());
}
