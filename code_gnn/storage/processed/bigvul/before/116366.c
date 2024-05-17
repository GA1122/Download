void ReadAndVerifyTransaction(net::HttpTransaction* trans,
                              const MockTransaction& trans_info) {
  std::string content;
  int rv = ReadTransaction(trans, &content);

  EXPECT_EQ(net::OK, rv);
  std::string expected(trans_info.data);
  EXPECT_EQ(expected, content);
}
