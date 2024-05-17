  void VerifyTestDataInEntry(BaseTransaction* trans, Entry* entry) {
    EXPECT_FALSE(entry->Get(IS_DIR));
    EXPECT_FALSE(entry->Get(IS_DEL));
    VerifyTestBookmarkDataInEntry(entry);
  }
