void Histogram::WriteAsciiImpl(bool graph_it,
                               const std::string& newline,
                               std::string* output) const {
  std::unique_ptr<SampleVector> snapshot = SnapshotSampleVector();
  Count sample_count = snapshot->TotalCount();

  WriteAsciiHeader(*snapshot, sample_count, output);
  output->append(newline);

  double max_size = 0;
  if (graph_it)
    max_size = GetPeakBucketSize(*snapshot);

  uint32_t largest_non_empty_bucket = bucket_count() - 1;
  while (0 == snapshot->GetCountAtIndex(largest_non_empty_bucket)) {
    if (0 == largest_non_empty_bucket)
      break;   
    --largest_non_empty_bucket;
  }

  size_t print_width = 1;
  for (uint32_t i = 0; i < bucket_count(); ++i) {
    if (snapshot->GetCountAtIndex(i)) {
      size_t width = GetAsciiBucketRange(i).size() + 1;
      if (width > print_width)
        print_width = width;
    }
  }

  int64_t remaining = sample_count;
  int64_t past = 0;
  for (uint32_t i = 0; i < bucket_count(); ++i) {
    Count current = snapshot->GetCountAtIndex(i);
    if (!current && !PrintEmptyBucket(i))
      continue;
    remaining -= current;
    std::string range = GetAsciiBucketRange(i);
    output->append(range);
    for (size_t j = 0; range.size() + j < print_width + 1; ++j)
      output->push_back(' ');
    if (0 == current && i < bucket_count() - 1 &&
        0 == snapshot->GetCountAtIndex(i + 1)) {
      while (i < bucket_count() - 1 &&
             0 == snapshot->GetCountAtIndex(i + 1)) {
        ++i;
      }
      output->append("... ");
      output->append(newline);
      continue;   
    }
    double current_size = GetBucketSize(current, i);
    if (graph_it)
      WriteAsciiBucketGraph(current_size, max_size, output);
    WriteAsciiBucketContext(past, current, remaining, i, output);
    output->append(newline);
    past += current;
  }
  DCHECK_EQ(sample_count, past);
}
