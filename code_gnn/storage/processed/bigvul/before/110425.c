void CheckParseUniformName(
    const char* name,
    bool expected_success,
    size_t expected_array_pos,
    int expected_index,
    bool expected_getting_array) {
  int index = 1234;
  size_t array_pos = 1244;
  bool getting_array = false;
  bool success = GLES2Util::ParseUniformName(
      name, &array_pos, &index, &getting_array);
  EXPECT_EQ(expected_success, success);
  if (success) {
    EXPECT_EQ(expected_array_pos, array_pos);
    EXPECT_EQ(expected_index, index);
    EXPECT_EQ(expected_getting_array, getting_array);
  }
}
