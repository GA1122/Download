  void CreateProxy(uint32 flags, FileProxy* proxy) {
    proxy->CreateOrOpen(
        test_path(), flags,
        Bind(&FileProxyTest::DidCreateOrOpen, weak_factory_.GetWeakPtr()));
    MessageLoop::current()->Run();
    EXPECT_TRUE(proxy->IsValid());
  }
