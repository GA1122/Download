scoped_refptr<LoginDelegate> ResourceDispatcherHostImpl::CreateLoginDelegate(
    ResourceLoader* loader,
    net::AuthChallengeInfo* auth_info) {
  if (!delegate_)
    return nullptr;

  net::URLRequest* request = loader->request();

  ResourceRequestInfoImpl* resource_request_info =
      ResourceRequestInfoImpl::ForRequest(request);
  DCHECK(resource_request_info);
  bool is_request_for_main_frame =
      resource_request_info->GetResourceType() == RESOURCE_TYPE_MAIN_FRAME;
  GlobalRequestID request_id = resource_request_info->GetGlobalRequestID();

  GURL url = request->url();

  scoped_refptr<LoginDelegate> login_delegate =
      GetContentClient()->browser()->CreateLoginDelegate(
          auth_info, resource_request_info->GetWebContentsGetterForRequest(),
          request_id, is_request_for_main_frame, url,
          request->response_headers(),
          resource_request_info->first_auth_attempt(),
          base::BindOnce(&ResourceDispatcherHostImpl::RunAuthRequiredCallback,
                         base::Unretained(this), request_id));

  resource_request_info->set_first_auth_attempt(false);

  return login_delegate;
}
