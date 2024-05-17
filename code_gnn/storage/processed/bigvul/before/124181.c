bool ContentBrowserClient::AllowWorkerIndexedDB(
    const GURL& url,
    const string16& name,
    ResourceContext* context,
    const std::vector<std::pair<int, int> >& render_views) {
  return true;
}
