  virtual void Register(const invalidation::ObjectId& oid) {
    EXPECT_FALSE(ContainsKey(registered_ids_, oid));
    registered_ids_.insert(oid);
  }
