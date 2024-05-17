  virtual void TearDown() {
    EXPECT_TRUE(RenderProcessHost::AllHostsIterator().IsAtEnd());

    GetContentClient()->set_browser_for_testing(old_browser_client_);

    DrainMessageLoops();
  }
