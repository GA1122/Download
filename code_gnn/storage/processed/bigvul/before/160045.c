void BackendImpl::GetStats(StatsItems* stats) {
  if (disabled_)
    return;

  std::pair<std::string, std::string> item;

  item.first = "Entries";
  item.second = base::IntToString(data_->header.num_entries);
  stats->push_back(item);

  item.first = "Pending IO";
  item.second = base::IntToString(num_pending_io_);
  stats->push_back(item);

  item.first = "Max size";
  item.second = base::IntToString(max_size_);
  stats->push_back(item);

  item.first = "Current size";
  item.second = base::IntToString(data_->header.num_bytes);
  stats->push_back(item);

  item.first = "Cache type";
  item.second = "Blockfile Cache";
  stats->push_back(item);

  stats_.GetItems(stats);
}
