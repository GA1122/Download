void DraggedTabGtk::AnimationEnded(const Animation* animation) {
  animation_callback_->Run();
}
