void TopSitesCacheTest::BuildTopSites(const char** spec, size_t size) {
  std::set<std::string> urls_seen;
  for (size_t i = 0; i < size; ++i) {
    const char* spec_item = spec[i];
    while (*spec_item && *spec_item == ' ')   
      ++spec_item;
    if (urls_seen.find(spec_item) != urls_seen.end())
      NOTREACHED() << "Duplicate URL found: " << spec_item;
    urls_seen.insert(spec_item);
    if (spec_item == spec[i]) {   
      base::string16 title(base::ASCIIToUTF16("Title ") +
                     base::Uint64ToString16(top_sites_.size() + 1));
      top_sites_.push_back(MostVisitedURL(GURL(spec_item), title));
    }
    ASSERT_TRUE(!top_sites_.empty());
    top_sites_.back().redirects.push_back(GURL(spec_item));
  }
}
