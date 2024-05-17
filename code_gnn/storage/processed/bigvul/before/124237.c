ResourceRequestInfoImpl* ResourceDispatcherHostImpl::CreateRequestInfo(
    int child_id,
    int route_id,
    bool download,
    ResourceContext* context) {
  return new ResourceRequestInfoImpl(
      PROCESS_TYPE_RENDERER,
      child_id,
      route_id,
      0,
      request_id_,
      false,      
      -1,         
      false,      
      -1,         
      ResourceType::SUB_RESOURCE,
      PAGE_TRANSITION_LINK,
      download,   
      download,   
      false,      
      WebKit::WebReferrerPolicyDefault,
      context);
}
