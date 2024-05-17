void Dispatcher::OnSetWebViewPartitionID(const std::string& partition_id) {
  CHECK(webview_partition_id_.empty() || webview_partition_id_ == partition_id);
  webview_partition_id_ = partition_id;
}
