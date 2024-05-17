void CustomButton::StartThrobbing(int cycles_til_stop) {
  is_throbbing_ = true;
  hover_animation_->StartThrobbing(cycles_til_stop);
}
