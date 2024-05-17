void BookmarksCreateFunction::GetQuotaLimitHeuristics(
    QuotaLimitHeuristics* heuristics) const {
  BookmarksQuotaLimitFactory::BuildForCreate(heuristics, profile());
}
