  void ResetTopSites() {
    DestroyTopSites();
    DCHECK(!top_sites_impl_);
    PrepopulatedPageList prepopulated_pages;
    prepopulated_pages.push_back(PrepopulatedPage(GURL(kPrepopulatedPageURL),
                                                  base::string16(), -1, -1, 0));
    top_sites_impl_ = new TopSitesImpl(
        pref_service_.get(), history_service_.get(),
        std::make_unique<DefaultTopSitesProvider>(history_service_.get()),
        prepopulated_pages, base::Bind(MockCanAddURLToHistory));
    top_sites_impl_->Init(scoped_temp_dir_.GetPath().Append(kTopSitesFilename));
  }
