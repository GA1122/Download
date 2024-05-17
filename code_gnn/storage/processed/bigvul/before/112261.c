  void OnDone(const net::URLRequestStatus& status,
              int response_code,
              const std::string& response) {
    ASSERT_EQ(MessageLoop::current(), &message_loop_);
    status_ = status;
    response_code_ = response_code;
    response_ = response;
    message_loop_.PostTask(FROM_HERE, MessageLoop::QuitClosure());
  }
