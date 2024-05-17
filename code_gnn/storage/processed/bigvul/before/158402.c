  ExpectGestureScrollEventsAfterMouseWheelACK(
      bool is_first_ack,
      size_t enqueued_wheel_event_count) {
    MockWidgetInputHandler::MessageVector events =
        GetAndResetDispatchedMessages();
    std::string expected_events;
      if (is_first_ack)
        expected_events += "GestureScrollBegin GestureScrollUpdate ";
      for (size_t i = 0; i < enqueued_wheel_event_count; ++i)
        expected_events += "MouseWheel GestureScrollUpdate ";

    EXPECT_EQ(base::TrimWhitespaceASCII(expected_events, base::TRIM_TRAILING),
              GetMessageNames(events));
    return events;
  }
