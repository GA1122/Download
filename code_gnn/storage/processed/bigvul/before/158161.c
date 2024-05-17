  bool is_same_document() {
    EXPECT_EQ(1U, is_same_documents_.size());
    return is_same_documents_[0];
  }
