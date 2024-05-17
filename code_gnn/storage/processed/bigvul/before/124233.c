void ResourceDispatcherHostImpl::BeginSaveFile(
    const GURL& url,
    const Referrer& referrer,
    int child_id,
    int route_id,
    ResourceContext* context) {
  if (is_shutdown_)
    return;

  char url_buf[128];
  base::strlcpy(url_buf, url.spec().c_str(), arraysize(url_buf));
  base::debug::Alias(url_buf);
  CHECK(ContainsKey(active_resource_contexts_, context));

  scoped_ptr<ResourceHandler> handler(
      new SaveFileResourceHandler(child_id,
                                  route_id,
                                  url,
                                  save_file_manager_.get()));
  request_id_--;

  const net::URLRequestContext* request_context = context->GetRequestContext();
  bool known_proto =
      request_context->job_factory()->IsHandledURL(url);
  if (!known_proto) {
    NOTREACHED();
    return;
  }

  scoped_ptr<net::URLRequest> request(
      request_context->CreateRequest(url, NULL));
  request->set_method("GET");
  request->set_referrer(MaybeStripReferrer(referrer.url).spec());
  webkit_glue::ConfigureURLRequestForReferrerPolicy(request.get(),
                                                    referrer.policy);
  request->set_load_flags(net::LOAD_PREFERRING_CACHE);

  ResourceRequestInfoImpl* extra_info =
      CreateRequestInfo(child_id, route_id, false, context);
  extra_info->AssociateWithRequest(request.get());   

  BeginRequestInternal(request.Pass(), handler.Pass());
}
