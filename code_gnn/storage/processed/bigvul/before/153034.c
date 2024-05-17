std::vector<uint32_t> GetPageNumbersFromPrintPageNumberRange(
    const PP_PrintPageNumberRange_Dev* page_ranges,
    uint32_t page_range_count) {
  std::vector<uint32_t> page_numbers;
  for (uint32_t index = 0; index < page_range_count; ++index) {
    for (uint32_t page_number = page_ranges[index].first_page_number;
         page_number <= page_ranges[index].last_page_number; ++page_number) {
      page_numbers.push_back(page_number);
    }
  }
  return page_numbers;
}
