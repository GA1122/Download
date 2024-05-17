 void InputHandlerProxy::DispatchQueuedInputEvents() {
   base::TimeTicks now = tick_clock_->NowTicks();
  while (!compositor_event_queue_->empty()) {
    std::unique_ptr<EventWithCallback> event_with_callback =
        scroll_predictor_->ResampleScrollEvents(compositor_event_queue_->Pop(),
                                                now);

    DispatchSingleInputEvent(std::move(event_with_callback), now);
  }
}
