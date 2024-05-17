  virtual void DoFailByTooBigDataAndClose(SocketStreamEvent* event) {
    std::string frame(event->number + 1, 0x00);
    VLOG(1) << event->number;
    EXPECT_FALSE(event->socket->SendData(&frame[0], frame.size()));
    event->socket->Close();
  }
