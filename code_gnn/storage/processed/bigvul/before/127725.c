void TestInitFromValue(const std::string& values, bool expected_return_code,
                       int expected_extra_info_spec) {
  int actual_info_spec;
  bool actual_return_code = GenerateInfoSpec(values, &actual_info_spec);
  EXPECT_EQ(expected_return_code, actual_return_code);
  if (expected_return_code)
    EXPECT_EQ(expected_extra_info_spec, actual_info_spec);
}
