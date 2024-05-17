  static void ReadString(CompoundBufferInputStream* input,
                         const std::string& str) {
    SCOPED_TRACE(str);
    scoped_array<char> buffer(new char[str.size() + 1]);
    buffer[str.size()] = '\0';
    EXPECT_EQ(ReadFromInput(input, buffer.get(), str.size()), str.size());
    EXPECT_STREQ(str.data(), buffer.get());
  }
