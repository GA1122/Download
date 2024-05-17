  void ResourceFetcher::EnableIsPreloadedForTest() {
  if (preloaded_ur_ls_for_test_)
//   if (preloaded_urls_for_test_)
      return;
  preloaded_ur_ls_for_test_ = WTF::WrapUnique(new HashSet<String>);
//   preloaded_urls_for_test_ = WTF::WrapUnique(new HashSet<String>);
  
    if (preloads_) {
      for (const auto& resource : *preloads_)
      preloaded_ur_ls_for_test_->insert(resource->Url().GetString());
//       preloaded_urls_for_test_->insert(resource->Url().GetString());
    }
  }