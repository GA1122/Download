void SparseHistogram::WriteAsciiImpl(bool graph_it,
                                     const std::string& newline,
                                     std::string* output) const {
  std::unique_ptr<HistogramSamples> snapshot = SnapshotSamples();
  Count total_count = snapshot->TotalCount();
  double scaled_total_count = total_count / 100.0;

  WriteAsciiHeader(total_count, output);
  output->append(newline);

  Count largest_count = 0;
  Sample largest_sample = 0;
  std::unique_ptr<SampleCountIterator> it = snapshot->Iterator();
  while (!it->Done()) {
    Sample min;
    Sample max;
    Count count;
    it->Get(&min, &max, &count);
    if (min > largest_sample)
      largest_sample = min;
    if (count > largest_count)
      largest_count = count;
    it->Next();
  }
  size_t print_width = GetSimpleAsciiBucketRange(largest_sample).size() + 1;

  it = snapshot->Iterator();
  while (!it->Done()) {
    Sample min;
    Sample max;
    Count count;
    it->Get(&min, &max, &count);

    std::string range = GetSimpleAsciiBucketRange(min);
    output->append(range);
    for (size_t j = 0; range.size() + j < print_width + 1; ++j)
      output->push_back(' ');

    if (graph_it)
      WriteAsciiBucketGraph(count, largest_count, output);
    WriteAsciiBucketValue(count, scaled_total_count, output);
    output->append(newline);
    it->Next();
  }
}
