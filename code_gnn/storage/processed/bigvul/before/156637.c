  void AddDelayedResponse(const net::test_server::SendBytesCallback& send,
                          const net::test_server::SendCompleteCallback& done) {
    send_response_closures_.push_back(base::Bind(send, "", done));
  }
