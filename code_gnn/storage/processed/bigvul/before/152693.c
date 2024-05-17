CustomHistogram::CustomHistogram(const std::string& name,
                                 const BucketRanges* ranges)
    : Histogram(name,
                ranges->range(1),
                ranges->range(ranges->bucket_count() - 1),
                ranges) {}
