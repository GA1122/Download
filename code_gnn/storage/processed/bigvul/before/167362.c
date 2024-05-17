bool ProbablySameFilters(
    const base::Callback<bool(const GURL&)>& filter1,
    const base::Callback<bool(const GURL&)>& filter2) {
  return ProbablySameFilter(filter1).MatchAndExplain(filter2, nullptr);
}
