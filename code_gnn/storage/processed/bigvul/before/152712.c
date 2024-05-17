HistogramBase* LinearHistogram::FactoryGetWithRangeDescription(
    const std::string& name,
    Sample minimum,
    Sample maximum,
    uint32_t bucket_count,
    int32_t flags,
    const DescriptionPair descriptions[]) {
  bool valid_arguments = Histogram::InspectConstructionArguments(
      name, &minimum, &maximum, &bucket_count);
  DCHECK(valid_arguments);

  return Factory(name, minimum, maximum, bucket_count, flags, descriptions)
      .Build();
}
