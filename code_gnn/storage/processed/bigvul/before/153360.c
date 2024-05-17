Tab::~Tab() {
  close_button_->RemoveObserver(this);
}
