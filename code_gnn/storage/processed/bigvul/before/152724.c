void Histogram::GetCountAndBucketData(Count* count,
                                      int64_t* sum,
                                      ListValue* buckets) const {
  std::unique_ptr<SampleVector> snapshot = SnapshotSampleVector();
  *count = snapshot->TotalCount();
  *sum = snapshot->sum();
  uint32_t index = 0;
  for (uint32_t i = 0; i < bucket_count(); ++i) {
    Sample count_at_index = snapshot->GetCountAtIndex(i);
    if (count_at_index > 0) {
      std::unique_ptr<DictionaryValue> bucket_value(new DictionaryValue());
      bucket_value->SetInteger("low", ranges(i));
      if (i != bucket_count() - 1)
        bucket_value->SetInteger("high", ranges(i + 1));
      bucket_value->SetInteger("count", count_at_index);
      buckets->Set(index, bucket_value.release());
      ++index;
    }
  }
}
