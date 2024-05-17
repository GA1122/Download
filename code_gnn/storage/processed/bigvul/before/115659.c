  void SimulateClientConnection(int connection_index, bool authenticate) {
    scoped_refptr<protocol::ConnectionToClient> connection =
        (connection_index == 0) ? connection_ : connection2_;
    scoped_refptr<ClientSession> client = new ClientSession(
        host_.get(),
        connection,
        event_executor_,
        desktop_environment_->capturer());
    connection->set_host_stub(client.get());

    context_.network_message_loop()->PostTask(
        FROM_HERE, base::Bind(&ChromotingHostTest::AddClientToHost,
                              host_, client));
    if (authenticate) {
      context_.network_message_loop()->PostTask(
          FROM_HERE, base::Bind(&ClientSession::OnConnectionOpened,
                                client.get(), connection));
    }

    if (connection_index == 0) {
      client_ = client;
    } else {
      client2_ = client;
    }
  }
