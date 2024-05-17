  bool ScrollStateIsContentConsuming() const {
    return scroll_state() ==
           OverscrollController::ScrollState::CONTENT_CONSUMING;
  }
