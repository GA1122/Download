  virtual void Unregister(const invalidation::ObjectId& oid) {
    EXPECT_TRUE(ContainsKey(registered_ids_, oid));
    registered_ids_.erase(oid);
  }
