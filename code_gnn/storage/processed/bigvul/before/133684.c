  void expectEntry(size_t index, size_t size, const string& name,
                   const string& value) {
    HpackEntry* entry = decoder_peer_.header_table()->GetByIndex(index);
    EXPECT_EQ(name, entry->name()) << "index " << index;
    EXPECT_EQ(value, entry->value());
    EXPECT_EQ(size, entry->Size());
    EXPECT_EQ(index, decoder_peer_.header_table()->IndexOf(entry));
  }
