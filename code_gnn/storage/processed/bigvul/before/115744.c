  virtual void SetUp() {
    session_ = new protocol::FakeSession();
    session_->set_message_loop(&message_loop_);

    viewer_ = new ConnectionToClient(
        base::MessageLoopProxy::current(), session_);
    viewer_->set_host_stub(&host_stub_);
    viewer_->set_input_stub(&input_stub_);
    viewer_->SetEventHandler(&handler_);
    EXPECT_CALL(handler_, OnConnectionOpened(viewer_.get()));
    session_->state_change_callback().Run(
        protocol::Session::CONNECTED);
    session_->state_change_callback().Run(
        protocol::Session::CONNECTED_CHANNELS);
     message_loop_.RunAllPending();
   }
