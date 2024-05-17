LinearHistogram::LinearHistogram(const std::string& name,
                                 Sample minimum,
                                 Sample maximum,
                                 const BucketRanges* ranges)
    : Histogram(name, minimum, maximum, ranges) {
}
