DownloadItemImpl::DestinationInfo::DestinationInfo(
    const base::FilePath& target_path,
    const base::FilePath& current_path,
    int64_t received_bytes,
    bool all_data_saved,
    const std::string& hash,
    base::Time end_time)
    : target_path(target_path),
      current_path(current_path),
      received_bytes(received_bytes),
      all_data_saved(all_data_saved),
      hash(hash),
      end_time(end_time) {}
