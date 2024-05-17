ConfirmInfoBar::~ConfirmInfoBar() {
  if (!initialized_) {
    delete ok_button_;
    delete cancel_button_;
    delete link_;
  }
}
