 void ClientSession::LocalMouseMoved(const SkIPoint& mouse_pos) {
  std::list<SkIPoint>::iterator found_position =
      std::find(injected_mouse_positions_.begin(),
                injected_mouse_positions_.end(), mouse_pos);
  if (found_position != injected_mouse_positions_.end()) {
    injected_mouse_positions_.erase(injected_mouse_positions_.begin(),
                                    ++found_position);
  } else {
    latest_local_input_time_ = base::Time::Now();
  }
}
