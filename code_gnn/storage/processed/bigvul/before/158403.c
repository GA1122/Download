  ExpectGestureScrollUpdateAfterNonBlockingMouseWheelACK(
      bool wheel_was_queued) {
    MockWidgetInputHandler::MessageVector events =
        GetAndResetDispatchedMessages();
    size_t gesture_scroll_update_index;
    if (wheel_was_queued) {
      gesture_scroll_update_index = 0;
    } else {
      EXPECT_TRUE(events[0]->ToEvent());
      EXPECT_EQ(WebInputEvent::kMouseWheel,
                events[0]->ToEvent()->Event()->web_event->GetType());
      gesture_scroll_update_index = 1;
    }
    EXPECT_EQ(gesture_scroll_update_index + 1, events.size());
    EXPECT_TRUE(events[gesture_scroll_update_index]->ToEvent());
    EXPECT_EQ(WebInputEvent::kGestureScrollUpdate,
              events[gesture_scroll_update_index]
                  ->ToEvent()
                  ->Event()
                  ->web_event->GetType());
    return events;
  }
