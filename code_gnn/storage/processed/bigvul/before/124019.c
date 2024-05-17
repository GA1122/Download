  static void BuildWithMappers(QuotaLimitHeuristics* heuristics,
      BucketMapper* short_mapper, BucketMapper* long_mapper) {
    const Config kSustainedLimitConfig = {
      bookmarks::MAX_SUSTAINED_WRITE_OPERATIONS_PER_MINUTE,
      TimeDelta::FromMinutes(1)
    };
    heuristics->push_back(new SustainedLimit(
        TimeDelta::FromMinutes(10),
        kSustainedLimitConfig,
        short_mapper,
        "MAX_SUSTAINED_WRITE_OPERATIONS_PER_MINUTE"));

    const Config kTimedLimitConfig = {
      bookmarks::MAX_WRITE_OPERATIONS_PER_HOUR,
      TimeDelta::FromHours(1)
    };
    heuristics->push_back(new TimedLimit(
        kTimedLimitConfig,
        long_mapper,
        "MAX_WRITE_OPERATIONS_PER_HOUR"));
  }
