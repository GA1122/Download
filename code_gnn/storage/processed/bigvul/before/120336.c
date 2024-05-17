  void ReceivedAckImpl(bool prevent_defaulted) {
    scoped_ptr<ui::TouchEvent> event(queue_.front());
    dispatcher_->ProcessedTouchEvent(event.get(), window_,
        prevent_defaulted ? ui::ER_HANDLED : ui::ER_UNHANDLED);
    queue_.pop();
  }
