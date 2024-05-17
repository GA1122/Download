  virtual void SetUp() {
    encoder_ = new MockEncoder();

    session_ = new MockSession();
    EXPECT_CALL(*session_, SetStateChangeCallback(_));
    connection_ = new MockConnectionToClient(
        session_, &host_stub_, &event_executor_);
    connection_->SetEventHandler(&handler_);

    record_ = new ScreenRecorder(
        &message_loop_, &message_loop_,
        base::MessageLoopProxy::current(),
         &capturer_, encoder_);
   }
