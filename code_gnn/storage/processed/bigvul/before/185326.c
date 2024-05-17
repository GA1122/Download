  void CastCastView::ButtonPressed(views::Button* sender,
                                   const ui::Event& event) {
    DCHECK(sender == stop_button_);
  StopCast();
//   cast_config_delegate_->StopCasting();
  }