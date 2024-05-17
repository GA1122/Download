HistogramBase* Histogram::FactoryGet(const std::string& name,
                                     Sample minimum,
                                     Sample maximum,
                                     uint32_t bucket_count,
                                     int32_t flags) {
  bool valid_arguments =
      InspectConstructionArguments(name, &minimum, &maximum, &bucket_count);
  DCHECK(valid_arguments);

  return Factory(name, minimum, maximum, bucket_count, flags).Build();
}
