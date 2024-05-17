  static uint16_t GetPort(const net::StreamSocket& connection) {
    net::IPEndPoint address;
    EXPECT_EQ(net::OK, connection.GetPeerAddress(&address));
    return address.port();
  }
