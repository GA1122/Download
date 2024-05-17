void Textfield::SetBorder(std::unique_ptr<Border> b) {
  if (use_focus_ring_ && HasFocus())
    FocusRing::Uninstall(this);
  use_focus_ring_ = false;
  View::SetBorder(std::move(b));
}
