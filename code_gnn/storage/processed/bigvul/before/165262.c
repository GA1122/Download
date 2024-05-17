std::unique_ptr<NavigationRequest> NavigationRequest::CreateBrowserInitiated(
    FrameTreeNode* frame_tree_node,
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    bool browser_initiated,
    const std::string& extra_headers,
    const FrameNavigationEntry& frame_entry,
    NavigationEntryImpl* entry,
    const scoped_refptr<network::ResourceRequestBody>& post_body,
    std::unique_ptr<NavigationUIData> navigation_ui_data) {
  bool is_form_submission = !!post_body;

  auto navigation_params = mojom::BeginNavigationParams::New(
      extra_headers, net::LOAD_NORMAL, false  ,
      blink::mojom::RequestContextType::LOCATION,
      blink::WebMixedContentContextType::kBlockable, is_form_submission,
      GURL()  ,
      std::string()  ,
      GURL()  ,
      base::nullopt  );

  if (common_params.navigation_type ==
      FrameMsg_Navigate_Type::RELOAD_BYPASSING_CACHE) {
    navigation_params->load_flags |= net::LOAD_BYPASS_CACHE;
    navigation_params->skip_service_worker = true;
  }

  std::unique_ptr<NavigationRequest> navigation_request(new NavigationRequest(
      frame_tree_node, common_params, std::move(navigation_params),
      commit_params, browser_initiated, false  ,
      false  , &frame_entry, entry,
      std::move(navigation_ui_data), nullptr, nullptr));
  navigation_request->blob_url_loader_factory_ =
      frame_entry.blob_url_loader_factory();

  if (blink::BlobUtils::MojoBlobURLsEnabled() &&
      common_params.url.SchemeIsBlob() &&
      !navigation_request->blob_url_loader_factory_) {
    navigation_request->blob_url_loader_factory_ =
        ChromeBlobStorageContext::URLLoaderFactoryForUrl(
            frame_tree_node->navigator()->GetController()->GetBrowserContext(),
            common_params.url);
  }

  return navigation_request;
}
