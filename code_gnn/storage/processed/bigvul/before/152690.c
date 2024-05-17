BooleanHistogram::BooleanHistogram(const std::string& name,
                                   const BucketRanges* ranges)
    : LinearHistogram(name, 1, 2, ranges) {}
