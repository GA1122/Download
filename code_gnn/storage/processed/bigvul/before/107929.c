void InfoBar::AnimationProgressed(const ui::Animation* animation) {
  if (container_)
    container_->InfoBarAnimated(true);
}
