BooleanHistogram::BooleanHistogram(const std::string& name,
                                   const BucketRanges* ranges,
                                   HistogramBase::AtomicCount* counts,
                                   HistogramBase::AtomicCount* logged_counts,
                                   HistogramSamples::Metadata* meta,
                                   HistogramSamples::Metadata* logged_meta)
    : LinearHistogram(name, 1, 2, ranges, counts, logged_counts, 2, meta,
                      logged_meta) {}
