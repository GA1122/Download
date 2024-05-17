  static void WritesCorrectly(size_t num_chars) {
    std::string buffer;
    char kOriginal[] = "supercali";
    strncpy(WriteInto(&buffer, num_chars + 1), kOriginal, num_chars);
    EXPECT_EQ(std::string(kOriginal,
                          std::min(num_chars, arraysize(kOriginal) - 1)),
              std::string(buffer.c_str()));
    EXPECT_EQ(num_chars, buffer.size());
  }
