void InfoBarContainer::InfoBarAnimated(bool completed) {
  if (delegate_)
    delegate_->InfoBarContainerSizeChanged(!completed);
}
