void CheckCurrentNameValuePair(HttpUtil::NameValuePairsIterator* parser,
                               bool expect_valid,
                               std::string expected_name,
                               std::string expected_value) {
  ASSERT_EQ(expect_valid, parser->valid());
  if (!expect_valid) {
    return;
  }

  std::string::const_iterator first_value_begin = parser->value_begin();
  std::string::const_iterator first_value_end = parser->value_end();

  ASSERT_EQ(expected_name, std::string(parser->name_begin(),
                                       parser->name_end()));
  ASSERT_EQ(expected_name, parser->name());
  ASSERT_EQ(expected_value, std::string(parser->value_begin(),
                                        parser->value_end()));
  ASSERT_EQ(expected_value, parser->value());

  ASSERT_TRUE(first_value_begin == parser->value_begin());
  ASSERT_TRUE(first_value_end == parser->value_end());
}
