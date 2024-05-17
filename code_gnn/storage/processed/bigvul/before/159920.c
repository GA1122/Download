  void ContainsPrepopulatePages(const TopSitesQuerier& querier,
                                size_t start_index) {
    PrepopulatedPageList prepopulate_pages = GetPrepopulatedPages();
    ASSERT_LE(start_index + prepopulate_pages.size(), querier.urls().size());
    for (size_t i = 0; i < prepopulate_pages.size(); ++i) {
      EXPECT_EQ(prepopulate_pages[i].most_visited.url.spec(),
                querier.urls()[start_index + i].url.spec())
          << " @ index " << i;
    }
  }
