void ResourceDispatcherHost::BeginRequest(
    int request_id,
    const ViewHostMsg_Resource_Request& request_data,
    IPC::Message* sync_result,   
    int route_id) {
  ChildProcessInfo::ProcessType process_type = receiver_->type();
  int child_id = receiver_->id();
  ChromeURLRequestContext* context = static_cast<ChromeURLRequestContext*>(
      receiver_->GetRequestContext(request_id, request_data));
  if (!context) {
    URLRequestContextGetter* context_getter =
        Profile::GetDefaultRequestContext();
    if (context_getter) {
      context = static_cast<ChromeURLRequestContext*>(
          context_getter->GetURLRequestContext());
    }
  }

  if (is_shutdown_ ||
      !ShouldServiceRequest(process_type, child_id, request_data)) {
    URLRequestStatus status(URLRequestStatus::FAILED, net::ERR_ABORTED);
    if (sync_result) {
      SyncLoadResult result;
      result.status = status;
      ViewHostMsg_SyncLoad::WriteReplyParams(sync_result, result);
      receiver_->Send(sync_result);
    } else {
      receiver_->Send(new ViewMsg_Resource_RequestComplete(
          route_id,
          request_id,
          status,
          std::string()));   
    }
    return;
  }

  PluginService::GetInstance()->LoadChromePlugins(this);

  scoped_refptr<ResourceHandler> handler;
  if (sync_result) {
    handler = new SyncResourceHandler(receiver_, request_data.url, sync_result);
  } else {
    handler = new AsyncResourceHandler(receiver_,
                                       child_id,
                                       route_id,
                                       receiver_->handle(),
                                       request_data.url,
                                       this);
  }

  if (HandleExternalProtocol(request_id, child_id, route_id,
                             request_data.url, request_data.resource_type,
                             handler)) {
    return;
  }

  URLRequest* request = new URLRequest(request_data.url, this);
  request->set_method(request_data.method);
  request->set_first_party_for_cookies(request_data.first_party_for_cookies);
  request->set_referrer(CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kNoReferrers) ? std::string() : request_data.referrer.spec());
  request->SetExtraRequestHeaders(request_data.headers);

  int load_flags = request_data.load_flags;
  if (request_data.resource_type == ResourceType::MAIN_FRAME)
    load_flags |= net::LOAD_VERIFY_EV_CERT;
  request->set_load_flags(load_flags);
  request->set_context(context);

  if (ResourceType::IsFrame(request_data.resource_type)) {
    request->set_priority(net::HIGHEST);
  } else {
    switch (request_data.resource_type) {
      case ResourceType::STYLESHEET:
      case ResourceType::SCRIPT:
        request->set_priority(net::MEDIUM);
        break;
      case ResourceType::IMAGE:
        request->set_priority(net::LOWEST);
        break;
      default:
        request->set_priority(net::LOW);
    }
  }

  uint64 upload_size = 0;
  if (request_data.upload_data) {
    request->set_upload(request_data.upload_data);
    upload_size = request_data.upload_data->GetContentLength();
  }

  if (request_data.resource_type == ResourceType::MAIN_FRAME &&
      process_type == ChildProcessInfo::RENDER_PROCESS &&
      Singleton<CrossSiteRequestManager>::get()->
          HasPendingCrossSiteRequest(child_id, route_id)) {
    handler = new CrossSiteResourceHandler(handler,
                                           child_id,
                                           route_id,
                                           this);
  }

  handler = new BufferedResourceHandler(handler, this, request);

  if (safe_browsing_->enabled() &&
      safe_browsing_->CanCheckUrl(request_data.url)) {
    handler = new SafeBrowsingResourceHandler(handler,
                                              child_id,
                                              route_id,
                                              request_data.resource_type,
                                              safe_browsing_,
                                              this,
                                              receiver_);
  }

  ResourceDispatcherHostRequestInfo* extra_info =
      new ResourceDispatcherHostRequestInfo(
          handler,
          process_type,
          child_id,
          route_id,
          request_id,
          request_data.frame_origin,
          request_data.main_frame_origin,
          request_data.resource_type,
          upload_size,
          false,   
          ResourceType::IsFrame(request_data.resource_type),  
          request_data.host_renderer_id,
          request_data.host_render_view_id);
  ApplyExtensionMessageFilterPolicy(request_data.url,
                                    request_data.resource_type,
                                    extra_info);
  SetRequestInfo(request, extra_info);   
  chrome_browser_net::SetOriginProcessUniqueIDForRequest(
      request_data.origin_child_id, request);

  appcache::AppCacheInterceptor::SetExtraRequestInfo(
      request, context ? context->appcache_service() : NULL, child_id,
      request_data.appcache_host_id, request_data.resource_type);

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnablePrivacyBlacklists)) {
    request->SetUserData(&BlacklistRequestInfo::kURLRequestDataKey,
        new BlacklistRequestInfo(request_data.url, request_data.resource_type,
            context ? context->GetPrivacyBlacklist() : NULL));
  }

  BeginRequestInternal(request);
}
