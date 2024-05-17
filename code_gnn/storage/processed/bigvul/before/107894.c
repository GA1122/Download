SaveCCInfoConfirmInfoBar::~SaveCCInfoConfirmInfoBar() {
  if (!initialized_) {
    delete save_button_;
    delete dont_save_button_;
    delete link_;
  }
}
