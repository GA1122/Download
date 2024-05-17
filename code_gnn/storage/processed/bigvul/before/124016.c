  static void Build(QuotaLimitHeuristics* heuristics) {
    BuildWithMappers(heuristics, new BookmarkIdMapper<FunctionType>(),
                                 new BookmarkIdMapper<FunctionType>());
  }
