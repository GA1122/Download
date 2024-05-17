void NavigationURLLoaderImpl::OnReceiveResponse(
    scoped_refptr<network::ResourceResponse> response,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<NavigationData> navigation_data,
    const GlobalRequestID& global_request_id,
    bool is_download,
    bool is_stream,
    PreviewsState previews_state) {
  TRACE_EVENT_ASYNC_END2("navigation", "Navigation timeToResponseStarted", this,
                         "&NavigationURLLoaderImpl", this, "success", true);


  delegate_->OnResponseStarted(
      std::move(response), std::move(url_loader_client_endpoints),
      std::move(navigation_data), global_request_id,
      allow_download_ && is_download, is_stream, previews_state,
      request_controller_->TakeSubresourceLoaderParams());
}
