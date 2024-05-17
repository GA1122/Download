  void Connect() {
    CallbackRunLoop run_loop(runner());

    EXPECT_CALL(*client_peer_->quic_transport_delegate(), OnConnected())
        .WillOnce(FireCallback(run_loop.CreateCallback()));
    EXPECT_CALL(*server_peer_->quic_transport_delegate(), OnConnected())
        .WillOnce(FireCallback(run_loop.CreateCallback()));

    StartHandshake();
    run_loop.RunUntilCallbacksFired();
  }
