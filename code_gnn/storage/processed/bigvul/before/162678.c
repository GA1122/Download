std::string BytesToHexString(unsigned char bytes[16], int length) {
  EXPECT_TRUE(length == 0 || length == 4 || length == 16)
      << "Bad IP address length: " << length;
  std::string result;
  for (int i = 0; i < length; ++i) {
    result.push_back(kHexCharLookup[(bytes[i] >> 4) & 0xf]);
    result.push_back(kHexCharLookup[bytes[i] & 0xf]);
  }
  return result;
}
