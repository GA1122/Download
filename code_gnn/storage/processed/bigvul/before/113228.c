bool ShelfLayoutManager::IsVisible() const {
  return status_->IsVisible() && (state_.visibility_state == VISIBLE ||
      (state_.visibility_state == AUTO_HIDE &&
       state_.auto_hide_state == AUTO_HIDE_SHOWN));
}
