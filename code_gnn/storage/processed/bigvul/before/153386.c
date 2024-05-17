int TabStrip::FrameGrabWidth() const {
  constexpr int kApparentWidth = 50;
  return kApparentWidth - new_tab_button_->GetInsets().right();
}
