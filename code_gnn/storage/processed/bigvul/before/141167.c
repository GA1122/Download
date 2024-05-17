bool Document::NextFrameHasPendingRAF() const {
  return scripted_animation_controller_ &&
         scripted_animation_controller_->NextFrameHasPendingRAF();
}
