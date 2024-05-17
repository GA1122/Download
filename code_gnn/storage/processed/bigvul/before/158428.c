  void PressAndSetTouchActionAuto() {
    PressTouchPoint(0, 1);
    SendTouchEvent();
    widget_host_->input_router()->OnSetTouchAction(cc::kTouchActionAuto);
    MockWidgetInputHandler::MessageVector events =
        GetAndResetDispatchedMessages();
    EXPECT_EQ("TouchStart", GetMessageNames(events));
  }
