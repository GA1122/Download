HeadlessPrintManager::PageRangeTextToPages(base::StringPiece page_range_text,
                                           bool ignore_invalid_page_ranges,
                                           int pages_count,
                                           std::vector<int>* pages) {
  printing::PageRanges page_ranges;
  for (const auto& range_string :
       base::SplitStringPiece(page_range_text, ",", base::TRIM_WHITESPACE,
                              base::SPLIT_WANT_NONEMPTY)) {
    printing::PageRange range;
    if (range_string.find("-") == base::StringPiece::npos) {
      if (!base::StringToInt(range_string, &range.from))
        return SYNTAX_ERROR;
      range.to = range.from;
    } else if (range_string == "-") {
      range.from = 1;
      range.to = pages_count;
    } else if (range_string.starts_with("-")) {
      range.from = 1;
      if (!base::StringToInt(range_string.substr(1), &range.to))
        return SYNTAX_ERROR;
    } else if (range_string.ends_with("-")) {
      range.to = pages_count;
      if (!base::StringToInt(range_string.substr(0, range_string.length() - 1),
                             &range.from))
        return SYNTAX_ERROR;
    } else {
      auto tokens = base::SplitStringPiece(
          range_string, "-", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
      if (tokens.size() != 2 || !base::StringToInt(tokens[0], &range.from) ||
          !base::StringToInt(tokens[1], &range.to))
        return SYNTAX_ERROR;
    }

    if (range.from < 1 || range.from > range.to) {
      if (!ignore_invalid_page_ranges)
        return SYNTAX_ERROR;
      continue;
    }
    if (range.from > pages_count) {
      if (!ignore_invalid_page_ranges)
        return LIMIT_ERROR;
      continue;
    }

    if (range.to > pages_count)
      range.to = pages_count;

    range.from--;
    range.to--;
    page_ranges.push_back(range);
  }
  *pages = printing::PageRange::GetPages(page_ranges);
  return PRINT_NO_ERROR;
}
