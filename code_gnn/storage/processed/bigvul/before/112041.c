  void ExpectLocalItemsInServerOrder() {
    if (position_map_.empty())
      return;

    ReadTransaction trans(FROM_HERE, directory());

    Id prev_id;
    DCHECK(prev_id.IsRoot());
    PosMap::iterator next = position_map_.begin();
    for (PosMap::iterator i = next++; i != position_map_.end(); ++i) {
      Id id = i->second;
      Entry entry_with_id(&trans, GET_BY_ID, id);
      EXPECT_TRUE(entry_with_id.good());
      EXPECT_EQ(prev_id, entry_with_id.Get(PREV_ID));
      EXPECT_EQ(i->first, entry_with_id.Get(SERVER_POSITION_IN_PARENT));
      if (next == position_map_.end()) {
        EXPECT_EQ(Id(), entry_with_id.Get(NEXT_ID));
      } else {
        EXPECT_EQ(next->second, entry_with_id.Get(NEXT_ID));
        next++;
      }
      prev_id = id;
    }
  }
