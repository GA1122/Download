  virtual void GetBucketsForArgs(const ListValue* args, BucketList* buckets) {
    IdList ids;
    bool invalid_id = false;
    if (!FunctionType::ExtractIds(args, &ids, &invalid_id) || invalid_id)
      return;
    for (IdList::iterator it = ids.begin(); it != ids.end(); ++it)
      buckets->push_back(GetBucket(*it));
  }
