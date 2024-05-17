std::unique_ptr<NavigationRequest> NavigationRequest::CreateRendererInitiated(
    FrameTreeNode* frame_tree_node,
    NavigationEntryImpl* entry,
    const CommonNavigationParams& common_params,
    mojom::BeginNavigationParamsPtr begin_params,
    int current_history_list_offset,
    int current_history_list_length,
    bool override_user_agent,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory,
    mojom::NavigationClientAssociatedPtrInfo navigation_client) {
  DCHECK(FrameMsg_Navigate_Type::IsReload(common_params.navigation_type) ||
         common_params.navigation_type ==
             FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT);

  RequestNavigationParams request_params(
      override_user_agent,
      std::vector<GURL>(),   
      common_params.url, common_params.method,
      false,                           
      PageState(),                     
      0,                               
      false,                           
      std::map<std::string, bool>(),   
      false,   
      -1,      
      current_history_list_offset, current_history_list_length,
      false,   
      false  );
  std::unique_ptr<NavigationRequest> navigation_request(new NavigationRequest(
      frame_tree_node, common_params, std::move(begin_params), request_params,
      false,   
      true,    
      nullptr, entry,
      nullptr,   
      std::move(navigation_client)));
  navigation_request->blob_url_loader_factory_ =
      std::move(blob_url_loader_factory);
  return navigation_request;
}
