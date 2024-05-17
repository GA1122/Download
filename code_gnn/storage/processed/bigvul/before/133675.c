  const std::map<string, string>& DecodeBlockExpectingSuccess(StringPiece str) {
    EXPECT_TRUE(DecodeHeaderBlock(str));
    return decoded_block();
  }
