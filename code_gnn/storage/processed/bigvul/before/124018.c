  static void BuildForRemove(QuotaLimitHeuristics* heuristics,
                             Profile* profile) {
    BuildWithMappers(heuristics, new RemoveBookmarksBucketMapper(profile),
                                 new RemoveBookmarksBucketMapper(profile));
  }
