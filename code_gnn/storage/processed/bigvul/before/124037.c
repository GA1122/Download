void BookmarksMoveFunction::GetQuotaLimitHeuristics(
    QuotaLimitHeuristics* heuristics) const {
  BookmarksQuotaLimitFactory::Build<BookmarksMoveFunction>(heuristics);
}
