  void SendScrollUpdateAck(MockWidgetInputHandler::MessageVector& messages,
                           InputEventAckState ack_result) {
    for (size_t i = 0; i < messages.size(); ++i) {
      MockWidgetInputHandler::DispatchedEventMessage* event =
          messages[i]->ToEvent();
      if (event &&
          event->Event()->web_event->GetType() ==
              WebInputEvent::kGestureScrollUpdate &&
          event->HasCallback()) {
        event->CallCallback(ack_result);
        return;
      }
    }
    EXPECT_TRUE(false);
  }
