void InfoBar::RemoveInfoBar() const {
  if (container_)
    container_->RemoveDelegate(delegate());
}
