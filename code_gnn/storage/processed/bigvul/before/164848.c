bool HasDataAndName(const history::DownloadRow& row) {
  return row.received_bytes > 0 && !row.target_path.empty();
}
