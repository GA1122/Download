  void RunTwoGetTwoGetUserMediaWithDifferentContraints(
      const std::string& constraints1,
      const std::string& constraints2,
      const std::string& expected_result) {
    ASSERT_TRUE(embedded_test_server()->InitializeAndWaitUntilReady());

    GURL url(embedded_test_server()->GetURL("/media/getusermedia.html"));
    NavigateToURL(shell(), url);

    std::string command = "twoGetUserMedia(" + constraints1 + ',' +
        constraints2 + ')';

    EXPECT_EQ(expected_result, ExecuteJavascriptAndReturnResult(command));
  }
