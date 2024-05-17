 static void append_negative_gtest_filter(const char *str) {
   std::string filter = ::testing::FLAGS_gtest_filter;
 if (filter.find('-') == std::string::npos) filter += '-';
  filter += str;
 ::testing::FLAGS_gtest_filter = filter;
}
