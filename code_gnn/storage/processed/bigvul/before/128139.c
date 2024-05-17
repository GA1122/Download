void CastDetailedView::OnViewClicked(views::View* sender) {
  if (sender == footer()->content()) {
    TransitionToDefaultView();
  } else if (sender == options_) {
    cast_config_delegate_->LaunchCastOptions();
  } else {
    auto it = receiver_activity_map_.find(sender);
    if (it != receiver_activity_map_.end()) {
      cast_config_delegate_->CastToReceiver(it->second);
    }
  }
}
