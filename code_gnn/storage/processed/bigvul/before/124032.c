  Bucket* GetBucket(const BucketIdType& id) {
    Bucket* b = buckets_[id];
    if (b == NULL) {
      b = new Bucket();
      buckets_[id] = b;
    }
    return b;
  }
