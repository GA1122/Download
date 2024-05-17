  bool did_replace_entry() {
    EXPECT_EQ(1U, did_replace_entries_.size());
    return did_replace_entries_[0];
  }
