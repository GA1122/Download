void CustomButton::SetState(ButtonState state) {
  if (state == state_)
    return;

  if (animate_on_state_change_ &&
      (!is_throbbing_ || !hover_animation_->is_animating())) {
    is_throbbing_ = false;
    if ((state_ == STATE_HOVERED) && (state == STATE_NORMAL)) {
      hover_animation_->Hide();
    } else if (state != STATE_HOVERED) {
      hover_animation_->Reset();
    } else if (state_ == STATE_NORMAL) {
      hover_animation_->Show();
    } else {
      hover_animation_->Reset(1);
    }
  }

  state_ = state;
  StateChanged();
  SchedulePaint();
}
