ChromeContentBrowserClient::CreateNetworkContext(
    content::BrowserContext* context,
    bool in_memory,
    const base::FilePath& relative_partition_path) {
  Profile* profile = Profile::FromBrowserContext(context);
  return profile->CreateNetworkContext(in_memory, relative_partition_path);
}
