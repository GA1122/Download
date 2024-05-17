int BrowserActionsContainer::ContainerMinSize() const {
  return resize_gripper_->width() + chevron_->width() + kChevronRightMargin;
}
