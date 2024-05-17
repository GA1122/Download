  static void BuildForCreate(QuotaLimitHeuristics* heuristics,
                             Profile* profile) {
    BuildWithMappers(heuristics, new CreateBookmarkBucketMapper(profile),
                                 new CreateBookmarkBucketMapper(profile));
  }
