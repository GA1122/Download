ChromeContentBrowserClient::WillCreateURLLoaderRequestInterceptors(
    content::NavigationUIData* navigation_ui_data,
    int frame_tree_node_id,
    const scoped_refptr<network::SharedURLLoaderFactory>&
        network_loader_factory) {
  std::vector<std::unique_ptr<content::URLLoaderRequestInterceptor>>
      interceptors;
#if BUILDFLAG(ENABLE_OFFLINE_PAGES)
  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    interceptors.push_back(
        std::make_unique<offline_pages::OfflinePageURLLoaderRequestInterceptor>(
            navigation_ui_data, frame_tree_node_id));
  }
#endif

  if (base::FeatureList::IsEnabled(network::features::kNetworkService) &&
      base::FeatureList::IsEnabled(
          previews::features::kHTTPSServerPreviewsUsingURLLoader)) {
    interceptors.push_back(
        std::make_unique<previews::PreviewsLitePageURLLoaderInterceptor>(
            network_loader_factory, frame_tree_node_id));
  }

  return interceptors;
}
