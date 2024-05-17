  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const {
    heuristics->push_back(new TimedLimit(kFrozenConfig, new Mapper()));
  }
