  void DoSmoothWheelScroll(const gfx::Vector2d& distance) {
    blink::WebGestureEvent event =
        SyntheticWebGestureEventBuilder::BuildScrollBegin(
            distance.x(), -distance.y(), blink::WebGestureDevice::kTouchpad, 1);
    event.data.scroll_begin.delta_hint_units =
        ui::input_types::ScrollGranularity::kScrollByPixel;
    GetWidgetHost()->ForwardGestureEvent(event);

    const uint32_t kNumWheelScrolls = 2;
    for (uint32_t i = 0; i < kNumWheelScrolls; i++) {
      shell()->web_contents()->GetMainFrame()->InsertVisualStateCallback(
          base::BindOnce(&ScrollLatencyBrowserTest::InvokeVisualStateCallback,
                         base::Unretained(this)));

      blink::WebGestureEvent event2 =
          SyntheticWebGestureEventBuilder::BuildScrollUpdate(
              distance.x(), -distance.y(), 0,
              blink::WebGestureDevice::kTouchpad);
      event2.data.scroll_update.delta_units =
          ui::input_types::ScrollGranularity::kScrollByPixel;
      GetWidgetHost()->ForwardGestureEvent(event2);

      while (visual_state_callback_count_ <= i) {
        GiveItSomeTime();
      }
    }
  }
