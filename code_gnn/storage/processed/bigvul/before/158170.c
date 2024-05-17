void NavigationURLLoaderImpl::BindNonNetworkURLLoaderFactoryRequest(
    int frame_tree_node_id,
    const GURL& url,
    network::mojom::URLLoaderFactoryRequest factory) {
  auto it = non_network_url_loader_factories_.find(url.scheme());
  if (it == non_network_url_loader_factories_.end()) {
    DVLOG(1) << "Ignoring request with unknown scheme: " << url.spec();
    return;
  }

  url::Origin navigation_request_initiator = url::Origin();

  FrameTreeNode* frame_tree_node =
      FrameTreeNode::GloballyFindByID(frame_tree_node_id);
  auto* frame = frame_tree_node->current_frame_host();
  GetContentClient()->browser()->WillCreateURLLoaderFactory(
      frame->GetSiteInstance()->GetBrowserContext(), frame,
      true  , navigation_request_initiator, &factory,
      nullptr  );
  it->second->Clone(std::move(factory));
}
