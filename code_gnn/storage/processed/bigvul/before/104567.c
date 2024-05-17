  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const {
    heuristics->push_back(new SustainedLimit(TimeDelta::FromMinutes(5),
        k2PerMinute, new Mapper()));
    heuristics->push_back(new TimedLimit(k20PerHour, new Mapper()));
  }
