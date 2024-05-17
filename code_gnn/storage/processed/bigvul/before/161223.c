static void TestArray(SnifferTest* tests, size_t count) {
  std::string mime_type;

  for (size_t i = 0; i < count; ++i) {
    SniffMimeType(tests[i].content,
                       tests[i].content_len,
                       GURL(tests[i].url),
                       tests[i].type_hint,
                       &mime_type);
    EXPECT_EQ(tests[i].mime_type, mime_type);
  }
}
