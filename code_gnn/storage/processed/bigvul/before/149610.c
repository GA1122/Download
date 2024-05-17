  void CompleteHostLookup(const std::string& host, int result) {
    auto it = resolve_host_clients_.find(host);
    if (it == resolve_host_clients_.end()) {
      ADD_FAILURE() << host << " wasn't found";
      return;
    }
    it->second->OnComplete(result, base::nullopt);
    resolve_host_clients_.erase(it);
    base::RunLoop().RunUntilIdle();
  }
