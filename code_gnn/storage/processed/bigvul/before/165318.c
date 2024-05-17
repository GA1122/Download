ChromeContentBrowserClient::WillCreateURLLoaderRequestInterceptors(
    content::NavigationUIData* navigation_ui_data,
    int frame_tree_node_id) {
  std::vector<std::unique_ptr<content::URLLoaderRequestInterceptor>>
      interceptors;
#if BUILDFLAG(ENABLE_OFFLINE_PAGES)
  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    interceptors.push_back(
        std::make_unique<offline_pages::OfflinePageURLLoaderRequestInterceptor>(
            navigation_ui_data, frame_tree_node_id));
  }
#endif
  return interceptors;
}
