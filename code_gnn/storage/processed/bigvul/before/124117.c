bool ChromeContentBrowserClient::IsValidStoragePartitionId(
    content::BrowserContext* browser_context,
    const std::string& partition_id) {
  if (partition_id.empty())
    return true;

  return GURL(partition_id).is_valid();
}
