  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const {
    heuristics->push_back(new TimedLimit(k2PerMinute, new Mapper()));
  }
