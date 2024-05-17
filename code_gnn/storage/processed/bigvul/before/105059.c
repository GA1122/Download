bool LiveSyncTest::ServerSupportsErrorTriggering() {
  EXPECT_TRUE(server_type_ != SERVER_TYPE_UNDECIDED);

  return server_type_ == LOCAL_PYTHON_SERVER;
}
