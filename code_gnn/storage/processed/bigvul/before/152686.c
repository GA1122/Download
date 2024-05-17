void Histogram::AddCount(int value, int count) {
  DCHECK_EQ(0, ranges(0));
  DCHECK_EQ(kSampleType_MAX, ranges(bucket_count()));

  if (value > kSampleType_MAX - 1)
    value = kSampleType_MAX - 1;
  if (value < 0)
    value = 0;
  if (count <= 0) {
    NOTREACHED();
    return;
  }
  samples_->Accumulate(value, count);

  FindAndRunCallback(value);
}
