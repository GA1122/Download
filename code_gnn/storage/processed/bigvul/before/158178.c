  std::vector<std::unique_ptr<URLLoaderThrottle>> CreateURLLoaderThrottles() {
    return GetContentClient()->browser()->CreateURLLoaderThrottles(
        *resource_request_, resource_context_, web_contents_getter_,
        navigation_ui_data_.get(), frame_tree_node_id_);
  }
