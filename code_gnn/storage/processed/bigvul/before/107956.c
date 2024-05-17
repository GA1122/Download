void InfoBar::Open() {
  animation_->Reset(1.0);
  if (container_)
    container_->InfoBarAnimated(false);
}
