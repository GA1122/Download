  virtual void GetBucketsForArgs(const ListValue* args, BucketList* buckets) {
    for (size_t i = 0; i < args->GetSize(); i++) {
      int id;
      ASSERT_TRUE(args->GetInteger(i, &id));
      if (buckets_.find(id) == buckets_.end())
        buckets_[id] = new Bucket();
      buckets->push_back(buckets_[id]);
    }
  }
