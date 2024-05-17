  const std::vector<std::string>& GetDomains() {
    helper_->StartFetching(
        base::Bind(&RemovePluginDataTester::OnSitesWithFlashDataFetched,
                   base::Unretained(this)));
    return domains_;
  }
