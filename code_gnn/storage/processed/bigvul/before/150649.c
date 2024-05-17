void InputHandlerProxy::HandleInputEventWithLatencyInfo(
    WebScopedInputEvent event,
    const LatencyInfo& latency_info,
    EventDispositionCallback callback) {
  DCHECK(input_handler_);

  TRACE_EVENT_WITH_FLOW1("input,benchmark", "LatencyInfo.Flow",
                         TRACE_ID_DONT_MANGLE(latency_info.trace_id()),
                         TRACE_EVENT_FLAG_FLOW_IN | TRACE_EVENT_FLAG_FLOW_OUT,
                         "step", "HandleInputEventImpl");

  std::unique_ptr<EventWithCallback> event_with_callback =
      std::make_unique<EventWithCallback>(std::move(event), latency_info,
                                          tick_clock_->NowTicks(),
                                          std::move(callback));

  enum {
    NO_SCROLL_PINCH = 0,
    ONGOING_SCROLL_PINCH = 1,
    SCROLL_PINCH = 2,
  };
  if (!IsGestureScrollOrPinch(event_with_callback->event().GetType())) {
    base::ScopedSampleMetadata metadata("Input.GestureScrollOrPinch",
                                        NO_SCROLL_PINCH);
    DispatchSingleInputEvent(std::move(event_with_callback),
                             tick_clock_->NowTicks());
    return;
  }

  base::ScopedSampleMetadata metadata("Input.GestureScrollOrPinch",
                                      has_ongoing_compositor_scroll_or_pinch_
                                          ? ONGOING_SCROLL_PINCH
                                          : SCROLL_PINCH);
  if (has_ongoing_compositor_scroll_or_pinch_) {
    const auto& gesture_event = ToWebGestureEvent(event_with_callback->event());
    bool is_from_set_non_blocking_touch =
        gesture_event.SourceDevice() == blink::WebGestureDevice::kTouchscreen &&
        gesture_event.is_source_touch_event_set_non_blocking;
    bool is_scroll_end_from_wheel =
        gesture_event.SourceDevice() == blink::WebGestureDevice::kTouchpad &&
        gesture_event.GetType() == blink::WebGestureEvent::kGestureScrollEnd;
    bool scroll_update_has_blocking_wheel_source =
        gesture_event.SourceDevice() == blink::WebGestureDevice::kTouchpad &&
        gesture_event.GetType() ==
            blink::WebGestureEvent::kGestureScrollUpdate &&
        is_first_gesture_scroll_update_;
    if (gesture_event.GetType() ==
        blink::WebGestureEvent::kGestureScrollUpdate) {
      is_first_gesture_scroll_update_ = false;
    }
    if (is_from_set_non_blocking_touch || is_scroll_end_from_wheel ||
        scroll_update_has_blocking_wheel_source || synchronous_input_handler_) {
      compositor_event_queue_->Queue(std::move(event_with_callback),
                                     tick_clock_->NowTicks());
      DispatchQueuedInputEvents();
      return;
    }

    bool needs_animate_input = compositor_event_queue_->empty();
    compositor_event_queue_->Queue(std::move(event_with_callback),
                                   tick_clock_->NowTicks());
    if (needs_animate_input)
      input_handler_->SetNeedsAnimateInput();
    return;
  }

  DispatchSingleInputEvent(std::move(event_with_callback),
                           tick_clock_->NowTicks());
}
