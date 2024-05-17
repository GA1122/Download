void RenderFrameImpl::DidStartResponse(
    const GURL& response_url,
    int request_id,
    const network::ResourceResponseHead& response_head,
    content::ResourceType resource_type) {
  for (auto& observer : observers_)
    observer.DidStartResponse(response_url, request_id, response_head,
                              resource_type);
}
