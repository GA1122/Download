  void set_socket_stream(const scoped_refptr<SocketStream>& socket_stream) {
    socket_stream_ = socket_stream;
    EXPECT_EQ(socket_stream_->delegate(), this);
  }
