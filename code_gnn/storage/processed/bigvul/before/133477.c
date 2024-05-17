OobeUI::~OobeUI() {
  core_handler_->SetDelegate(nullptr);
  network_dropdown_handler_->RemoveObserver(error_screen_handler_);
}
