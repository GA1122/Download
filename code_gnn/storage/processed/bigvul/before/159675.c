  void StartServer() {
    ASSERT_TRUE(embedded_test_server()->Start());

    foo_host_port_ = embedded_test_server()->host_port_pair();
    foo_host_port_.set_host(foo_com_);
  }
