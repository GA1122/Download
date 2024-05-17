void ResourcePrefetchPredictor::DeleteUrls(const history::URLRows& urls) {
  std::vector<std::string> hosts_to_delete;

  for (const auto& it : urls)
    hosts_to_delete.emplace_back(it.url().host());

  host_redirect_data_->DeleteData(hosts_to_delete);
  origin_data_->DeleteData(hosts_to_delete);
}
